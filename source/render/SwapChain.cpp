#include "SwapChain.hpp"

#include <limits>
#include <array>

#include "spdlog/spdlog.h"

#include "Device.hpp"
#include "Image.hpp"
#include "../Constants.hpp"
#include "../tools/Tools.hpp"


namespace lve {

    SwapChain::SwapChain(const std::shared_ptr<Device>& device, const vk::Extent2D &windowExtent, const vk::SurfaceKHR& surface) {
        logicalDevice = device->getLogicalDevice();
        graphicsQueue = device->getGraphicsQueue();
        presentQueue =  device->getPresentQueue();
        vk::Format depthFormat = device->findSupportFormat(
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );

        createSwapChain(device, surface);
        createImageViews();
        createRenderPass(depthFormat);
        createDepthResources(device, depthFormat);
        createFramebuffers();
        createSyncObjects();
    }

    SwapChain::SwapChain(const std::shared_ptr<Device> &device, const vk::Extent2D &windowExtent, const vk::SurfaceKHR& surface,
                         const std::shared_ptr<SwapChain>& oldSwapChain) {
        logicalDevice = oldSwapChain->logicalDevice;
        graphicsQueue = oldSwapChain->graphicsQueue;
        presentQueue = oldSwapChain->presentQueue;

        vk::Format depthFormat = device->findSupportFormat(
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );

        createSwapChain(device, surface, oldSwapChain);
        createImageViews();
        createRenderPass(depthFormat);
        createDepthResources(device, depthFormat);
        createFramebuffers();
        createSyncObjects();

        oldSwapChain->cleanup(device->getAllocator());
    }

    SwapChain::~SwapChain() = default;

    void SwapChain::cleanup(const vma::Allocator& allocator) {
        for (auto& framebuffer : framebuffers)
            logicalDevice.destroy(framebuffer);

        logicalDevice.destroy(renderPass);

        for (auto& image : images)
            logicalDevice.destroy(image.view);

        for (auto& image : depthImages)
            image.destroy(logicalDevice, allocator);

        if (handle) {
            logicalDevice.destroy(handle);
            handle = VK_NULL_HANDLE;
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            logicalDevice.destroy(renderFinishedSemaphores[i]);
            logicalDevice.destroy(imageAvailableSemaphores[i]);
            logicalDevice.destroy(inFlightFences[i]);
        }
    }

    vk::Framebuffer SwapChain::getFrameBuffer(size_t index) {
        return framebuffers[index];
    }

    vk::RenderPass SwapChain::getRenderPass() {
        return renderPass;
    }

    vk::ImageView SwapChain::getImageView(size_t index) {
        return images[index].getView();
    }

    size_t SwapChain::imageCount() {
        return images.size();
    }

    vk::Format SwapChain::getFormat() {
        return format;
    }

    vk::Extent2D SwapChain::getExtent() {
        return extent;
    }

    uint32_t SwapChain::width() const {
        return extent.width;
    }

    uint32_t SwapChain::height() const {
        return extent.height;
    }

    float SwapChain::extentAspectRatio() const {
        return static_cast<float>(extent.width) / static_cast<float>(extent.height);
    }

    vk::Result SwapChain::acquireNextImage(uint32_t* imageIndex) {
        logicalDevice.waitForFences(
                1,
                &inFlightFences[currentFrame],
                true,
                std::numeric_limits<uint64_t>::max()
        );

        vk::ResultValue result = logicalDevice.acquireNextImageKHR(
                handle,
                std::numeric_limits<uint64_t>::max(),
                imageAvailableSemaphores[currentFrame],
                nullptr
        );

        *imageIndex = result.value;
        return result.result;
    }

    vk::Result SwapChain::submitCommandBuffer(const vk::CommandBuffer& cmdBuffer, uint32_t imageIndex) {
        if (imagesInFlight[imageIndex])
            logicalDevice.waitForFences(1, &imagesInFlight[imageIndex], true, std::numeric_limits<uint64_t>::max());

        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        vk::Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::Semaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        vk::SubmitInfo submitInfo(1, waitSemaphores, waitStages, 1, &cmdBuffer, 1, signalSemaphores);

        logicalDevice.resetFences(inFlightFences[currentFrame]);
        VK_HPP_CHECK_RESULT(graphicsQueue.submit(1, &submitInfo, inFlightFences[currentFrame]),
                            "Failed to submit draw command Buffer!");

        // Vulkan-Hpp throw a exception if result is VK_ERROR_OUT_OF_DATE_KHR, so there is not possible handle the result
        // that present return. Is preferable use the C-API instead.
        VkPresentInfoKHR presentInfo = vk::PresentInfoKHR(1, signalSemaphores, 1, &handle, &imageIndex);
        auto result = static_cast<vk::Result>(vkQueuePresentKHR(presentQueue, &presentInfo));

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    SwapChain::SupportDetails SwapChain::querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface) {
        SupportDetails details;
        details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
        details.formats = device.getSurfaceFormatsKHR(surface);
        details.presentModes = device.getSurfacePresentModesKHR(surface);

        return details;
    }

    void SwapChain::createSwapChain(const std::shared_ptr<Device>& device, vk::SurfaceKHR surface, const std::shared_ptr<SwapChain>& oldSwapChain) {
        SupportDetails supportDetails = querySwapChainSupport(device->getPhysicalDevice(), surface);
        vk::SurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(supportDetails.formats);
        vk::PresentModeKHR presentMode = choosePresentMode(supportDetails.presentModes);
        extent = chooseExtent(supportDetails.capabilities);

        uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;
        if (supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount) {
            imageCount = supportDetails.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR createInfo(
                {}, // flags
                surface,
                imageCount,
                surfaceFormat.format,
                surfaceFormat.colorSpace,
                extent,
                1, // imageArrayLayers
                vk::ImageUsageFlagBits::eColorAttachment
        );

        Device::QueueFamilyIndices indices = device->getQueueFamilyIndices();
        if (indices.graphics != indices.present) {
            uint32_t queueFamilyIndices[] = {indices.graphics, indices.present};
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = true;
        createInfo.oldSwapchain = oldSwapChain == nullptr ? nullptr : oldSwapChain->handle;

        handle = logicalDevice.createSwapchainKHR(createInfo);

        std::vector<vk::Image> swapChainImages = logicalDevice.getSwapchainImagesKHR(handle);
        images.resize(imageCount);

        for (size_t i = 0; i < imageCount; ++i)
            images[i].handle = swapChainImages[i];

        format = surfaceFormat.format;
    }

    void SwapChain::createImageViews() {
        for (auto& image : images) {
            vk::ImageViewCreateInfo createInfo{
                    {}, // flags
                    image.handle,
                    vk::ImageViewType::e2D,
                    format,
                    {}, // components
                    {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1} // subresourceRange
            };

            image.view = logicalDevice.createImageView(createInfo);
        }
    }

    void SwapChain::createDepthResources(const std::shared_ptr<Device>& device, vk::Format depthFormat) {
        depthImages.resize(images.size());
        for (auto& image : depthImages) {
            vk::ImageCreateInfo imageInfo(
                    {}, // flags
                    vk::ImageType::e2D,
                    depthFormat,
                    {extent.width, extent.height, 1}, // extent
                    1,
                    1,
                    vk::SampleCountFlagBits::e1,
                    vk::ImageTiling::eOptimal,
                    vk::ImageUsageFlagBits::eDepthStencilAttachment,
                    vk::SharingMode::eExclusive,
                    0, // queueFamilyIndexCount
                    nullptr, // pQueueFamilyIndices
                    vk::ImageLayout::eUndefined
            );

            image = Image(device->getAllocator(), imageInfo, vma::MemoryUsage::eGpuOnly);
            image.createView(logicalDevice, vk::ImageAspectFlagBits::eDepth);
        }
    }

    void SwapChain::createRenderPass(vk::Format depthFormat) {
        vk::AttachmentDescription colorAttachment(
                {},
                format,
                vk::SampleCountFlagBits::e1,
                vk::AttachmentLoadOp::eClear,
                vk::AttachmentStoreOp::eStore,
                vk::AttachmentLoadOp::eDontCare, // stencil
                vk::AttachmentStoreOp::eDontCare, // stencil
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::ePresentSrcKHR
        );

        vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

        vk::AttachmentDescription depthAttachment(
                {},
                depthFormat,
                vk::SampleCountFlagBits::e1,
                vk::AttachmentLoadOp::eClear,
                vk::AttachmentStoreOp::eDontCare,
                vk::AttachmentLoadOp::eDontCare, // stencil
                vk::AttachmentStoreOp::eDontCare, // stencil
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eDepthStencilAttachmentOptimal
        );

        vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

        vk::SubpassDescription subpass(
                {},
                vk::PipelineBindPoint::eGraphics,
                0, // inputAttachmentCount
                nullptr, // pInputAttachments
                1, // pColorAttachmentCount
                &colorAttachmentRef, // pColorAttachments
                nullptr, // pResolverAttachments
                &depthAttachmentRef // pDepthStencilAttachment
        );

        vk::SubpassDependency dependency(
                VK_SUBPASS_EXTERNAL, // srcSubpass
                0, // dstSubpass
                vk::PipelineStageFlagBits::eColorAttachmentOutput, // srcStageMask
                vk::PipelineStageFlagBits::eColorAttachmentOutput, // dstStageMask
                {}, // srcAccessMask
                vk::AccessFlagBits::eColorAttachmentWrite, // dstAccessMask
                {} // subpassFlags
        );

        std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
        vk::RenderPassCreateInfo createInfo(
                {}, // flags
                castU32(attachments.size()),
                attachments.data(),
                1,
                &subpass,
                1,
                &dependency
        );

        renderPass = logicalDevice.createRenderPass(createInfo);
    }

    void SwapChain::createFramebuffers() {
        framebuffers.resize(images.size());
        for (size_t i = 0; i < images.size(); ++i) {
            std::array<vk::ImageView, 2> attachments = {images[i].view, depthImages[i].view};
            vk::FramebufferCreateInfo createInfo(
                    {}, // flags
                    renderPass,
                    castU32(attachments.size()),
                    attachments.data(),
                    extent.width,
                    extent.height,
                    1 // layers
            );

            framebuffers[i] = logicalDevice.createFramebuffer(createInfo);
        }
    }

    void SwapChain::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(images.size(), VK_NULL_HANDLE);

        vk::SemaphoreCreateInfo semaphoreInfo{};
        vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
            imageAvailableSemaphores[i] = logicalDevice.createSemaphore(semaphoreInfo);
            renderFinishedSemaphores[i] = logicalDevice.createSemaphore(semaphoreInfo);
            inFlightFences[i] = logicalDevice.createFence(fenceInfo);
        }
    }

    vk::SurfaceFormatKHR SwapChain::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
        for (auto& availableFormat : availableFormats) {
            if (availableFormat.format == vk::Format::eR8G8B8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
                return availableFormat;
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR SwapChain::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        for (auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                spdlog::info("Present mode: Mailbox - {}", vk::to_string(availablePresentMode));
                return availablePresentMode;
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapChain::chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            vk::Extent2D actualExtent = windowExtent;
            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                          std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

} //  namespace lve