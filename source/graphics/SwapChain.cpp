#include "SwapChain.hpp"

#include <utility>

#include "spdlog/spdlog.h"

#include "Image.hpp"
#include "Device.hpp"
#include "utils/Macros.hpp"


namespace lve {

    SwapChain::SwapChain(std::shared_ptr<Device> device, VkExtent2D windowExtent, VkSurfaceKHR surface)
            : device(std::move(device)), windowExtent(windowExtent), surface(surface) {
        logicalDevice = this->device->getLogicalDevice();
        this->device->getGraphicsQueue(graphicsQueue);
        this->device->getPresentQueue(presentQueue);

        init();
    }

    SwapChain::SwapChain(std::shared_ptr<Device> device, VkExtent2D windowExtent, VkSurfaceKHR surface, std::shared_ptr<SwapChain> oldSwapChain)
            : device(std::move(device)), windowExtent(windowExtent), surface(surface), oldSwapChain(std::move(oldSwapChain)) {
        logicalDevice = this->device->getLogicalDevice();
        graphicsQueue = this->oldSwapChain->graphicsQueue;
        presentQueue = this->oldSwapChain->presentQueue;

        init();

        oldSwapChain->destroy();
        oldSwapChain = nullptr;
    }

    SwapChain::~SwapChain() = default;

    void SwapChain::destroy() {
        for (auto& image : images)
            vkDestroyImageView(logicalDevice, image.view, nullptr);

        images.clear();

        if (swapChain) {
            vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
            swapChain = nullptr;
        }

        for (auto& image : depthImages)
            image->destroy();

        for (auto& framebuffer : framebuffers)
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);

        vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
        }
    }

    VkFramebuffer SwapChain::getFrameBuffer(size_t index) {
        return framebuffers[index];
    }

    VkRenderPass SwapChain::getRenderPass() {
        return renderPass;
    }

    VkImageView SwapChain::getImageView(size_t index) {
        return images[index].view;
    }

    size_t SwapChain::imageCount() {
        return images.size();
    }

    VkFormat SwapChain::getFormat() {
        return format;
    }

    VkExtent2D SwapChain::getExtent() {
        return extent;
    }

    uint32_t SwapChain::width() const {
        return extent.width;
    }

    uint32_t SwapChain::height() const {
        return extent.height;
    }

    float SwapChain::extentAspectRatio() const {
        return CAST_FLOAT(extent.width) / CAST_FLOAT(extent.height);
    }

    VkResult SwapChain::acquireNextImage(uint32_t *imageIndex) {
        vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        return vkAcquireNextImageKHR(
                logicalDevice,
                swapChain,
                std::numeric_limits<uint64_t>::max(),
                imageAvailableSemaphores[currentFrame],
                VK_NULL_HANDLE,
                imageIndex
        );
    }

    VkResult SwapChain::submitCommandBuffers(VkCommandBuffer const &commandBuffer, uint32_t *imageIndex) {
        if (imagesInFlight[*imageIndex])
            vkWaitForFences(logicalDevice, 1, &inFlightFences[*imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());

        imagesInFlight[*imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);
        LVE_VK_CHECK_RESULT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]),
                            "failed to submit draw command buffer!");

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(presentQueue, &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    bool SwapChain::compareFormats(const SwapChain &other) {
        return other.depthFormat == depthFormat && other.format == format;
    }

    SwapChain::SupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    void SwapChain::init() {
        depthFormat =  device->findSupportFormat(
                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        createSwapChain();
        createImageViews();
        createRenderPass();
        createDepthResources();
        createFramebuffers();
        createSyncObjects();
    }

    void SwapChain::createSwapChain() {
        SupportDetails support = querySwapChainSupport(device->getPhysicalDevice(), surface);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(support.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(support.presentModes);
        VkExtent2D extent_ = chooseSwapExtent(support.capabilities);

        uint32_t imageCount = support.capabilities.minImageCount + 1;
        if (support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount)
            imageCount = support.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent_;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Device::QueueFamilyIndices indices = device->getQueueFamilyIndices();
        if (indices.graphics != indices.present) {
            uint32_t queueFamilyIndices[] = {indices.graphics, indices.present};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = support.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = ( !oldSwapChain ? VK_NULL_HANDLE : oldSwapChain->swapChain);

        LVE_VK_CHECK_RESULT(vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain),
                            "Failed to create swap chain!");

        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
        std::vector<VkImage> swapChainImages(imageCount);
        vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

        images.resize(imageCount);
        for (size_t i = 0; i < imageCount; ++i)
            images[i].image = swapChainImages[i];

        format = surfaceFormat.format;
        extent = extent_;
    }

    void SwapChain::createImageViews() {
        for (auto& image : images) {
            VkImageViewCreateInfo viewInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            viewInfo.image = image.image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            LVE_VK_CHECK_RESULT(vkCreateImageView(logicalDevice, &viewInfo, nullptr, &image.view),
                                "Failed to create texture image view!");
        }
    }

    void SwapChain::createDepthResources() {
        depthImages.resize(images.size());
        for (auto& image : depthImages) {
            VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = extent.width;
            imageInfo.extent.height = extent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            image = std::make_unique<Image>(logicalDevice, device->getAllocator(), imageInfo, VMA_MEMORY_USAGE_GPU_ONLY);
            image->createView(VK_IMAGE_ASPECT_DEPTH_BIT);
        }
    }

    void SwapChain::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = depthFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.dstSubpass = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

        std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
        renderPassInfo.attachmentCount = CAST_U32(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        LVE_VK_CHECK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass),
                            "Failed to create render pass!");
    }

    void SwapChain::createFramebuffers() {
        VkFramebufferCreateInfo framebufferInfo = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        framebuffers.resize(imageCount());
        for (size_t i = 0; i < imageCount(); i++) {
            std::array<VkImageView, 2> attachments = {images[i].view, depthImages[i]->view};
            framebufferInfo.attachmentCount = CAST_U32(attachments.size());
            framebufferInfo.pAttachments = attachments.data();

            LVE_VK_CHECK_RESULT(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &framebuffers[i]),
                                "failed to create framebuffer!");
        }
    }

    void SwapChain::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

        VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            LVE_VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]),
                                "Failed to create synchronization objects for a frame!");

            LVE_VK_CHECK_RESULT(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]),
                                "Failed to create synchronization objects for a frame!");

            LVE_VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]),
                                "Failed to create synchronization objects for a frame!");
        }
    }

    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                spdlog::info("Present mode: Mailbox");
                return availablePresentMode;
            }
        }

        spdlog::info("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;;
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = windowExtent;
            actualExtent.width = std::max(
                    capabilities.minImageExtent.width,
                    std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                    capabilities.minImageExtent.height,
                    std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        };
    }

} // namespace lve