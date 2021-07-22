#ifndef LITTLEVULKANENGINE_SOURCE_RENDER_SWAPCHAIN_HPP
#define LITTLEVULKANENGINE_SOURCE_RENDER_SWAPCHAIN_HPP


#include <vector>
#include <memory>

#define VULKAN_HPP_NO_NODISCARD_WARNINGS
#include "vulkan/vulkan.hpp"


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
        SwapChain(std::shared_ptr<Device> device, const vk::Extent2D& windowExtent, const vk::SurfaceKHR& surface);

        SwapChain(const SwapChain&) = delete;

        ~SwapChain();

        SwapChain& operator=(const SwapChain&) = delete;

        void cleanup();

        vk::Framebuffer getFrameBuffer(size_t index);

        vk::RenderPass getRenderPass();

        vk::ImageView getImageView(size_t index);

        size_t imageCount();

        vk::Format getFormat();

        vk::Format getDepthFormat();

        vk::Extent2D getExtent();

        [[nodiscard]] uint32_t width() const;

        [[nodiscard]] uint32_t height() const;

        [[nodiscard]] float extentAspectRatio() const;

        vk::Result acquireNextImage(uint32_t* imageIndex);

        vk::Result submitCommandBuffer(const vk::CommandBuffer& cmdBuffer, uint32_t imageIndex);

        static SupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

    private:
        void createSwapChain();

        void createImageViews();

        void createDepthResources();

        void createRenderPass();

        void createFramebuffers();

        void createSyncObjects();

        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

        vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    private:
        std::shared_ptr<Device> device;
        vk::SurfaceKHR surface;
        vk::SwapchainKHR handle;
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::Format format{};
        vk::Format depthFormat{};
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
