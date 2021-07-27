#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_SWAPCHAIN_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_SWAPCHAIN_HPP


#include <vector>
#include <memory>

#define VULKAN_HPP_NO_NODISCARD_WARNINGS
#include "vulkan/vulkan.hpp"
#include "../extras/vk_mem_alloc.hpp"


namespace lve {

    class Device;
    class Image;

    class SwapChain {
    public:
        struct SupportDetails {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

    public:
        SwapChain(const std::shared_ptr<Device>& device, const vk::Extent2D& windowExtent, const vk::SurfaceKHR& surface);

        SwapChain(const std::shared_ptr<Device>& device, const vk::Extent2D& windowExtent, const vk::SurfaceKHR& surface,
                  const std::shared_ptr<SwapChain>& oldSwapChain);

        SwapChain(const SwapChain&) = delete;

        ~SwapChain();

        SwapChain& operator=(const SwapChain&) = delete;

        void cleanup(const vma::Allocator& allocator);

        vk::Framebuffer getFrameBuffer(size_t index);

        vk::RenderPass getRenderPass();

        vk::ImageView getImageView(size_t index);

        size_t imageCount();

        vk::Format getFormat();

        vk::Extent2D getExtent();

        [[nodiscard]] uint32_t width() const;

        [[nodiscard]] uint32_t height() const;

        [[nodiscard]] float extentAspectRatio() const;

        vk::Result acquireNextImage(uint32_t* imageIndex);

        vk::Result submitCommandBuffer(const vk::CommandBuffer& cmdBuffer, uint32_t imageIndex);

        bool compareFormats(const SwapChain& swapChain);

        static SupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

    private:
        void createSwapChain(const std::shared_ptr<Device>& device, vk::SurfaceKHR surface, const std::shared_ptr<SwapChain>& oldSwapChain = nullptr);

        void createImageViews();

        void createDepthResources(const std::shared_ptr<Device>& device);

        void createRenderPass();

        void createFramebuffers();

        void createSyncObjects();

        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

        vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    private:
        vk::Device logicalDevice;

        vk::SwapchainKHR handle;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::Format format{};
        vk::Format depthFormat;

        vk::Extent2D extent;
        vk::Extent2D windowExtent;

        vk::RenderPass renderPass;
        std::vector<vk::Framebuffer> framebuffers;

        std::vector<Image> images;
        std::vector<Image> depthImages;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        std::vector<vk::Fence> imagesInFlight;

        uint32_t currentFrame{0};
    };

} //  namespace lve


#endif //LITTLEVULKANENGINE_SOURCE_RENDER_SWAPCHAIN_HPP
