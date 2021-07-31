#ifndef LITTLEVULKANENGINE_SWAPCHAIN_HPP
#define LITTLEVULKANENGINE_SWAPCHAIN_HPP


#include <memory>
#include <vector>

#include "vulkan/vulkan.h"

#include "utils/NonCopyable.hpp"


namespace lve {

    class Device;
    class Image;

    class SwapChain : NonCopyable {
    public:
        struct SupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

    public:
        SwapChain(std::shared_ptr<Device> device, VkExtent2D windowExtent, VkSurfaceKHR surface);

        SwapChain(std::shared_ptr<Device> device, VkExtent2D windowExtent, VkSurfaceKHR surface,
                  std::shared_ptr<SwapChain> oldSwapChain);

        ~SwapChain() override;

        void destroy();

        VkFramebuffer getFrameBuffer(size_t index);

        VkRenderPass getRenderPass();

        VkImageView getImageView(size_t index);

        size_t imageCount();

        VkFormat getFormat();

        VkExtent2D getExtent();

        [[nodiscard]] uint32_t width() const;

        [[nodiscard]] uint32_t height() const;

        [[nodiscard]] float extentAspectRatio() const;

        VkResult acquireNextImage(uint32_t* imageIndex);

        VkResult submitCommandBuffers(const VkCommandBuffer& commandBuffer, const uint32_t* imageIndex);

        bool compareFormats(const SwapChain& other);

        static SupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    private:
        void init();

        void createSwapChain();

        void createImageViews();

        void createDepthResources();

        void createRenderPass();

        void createFramebuffers();

        void createSyncObjects();

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT{2};

    private:
        std::shared_ptr<Device> device;
        VkDevice logicalDevice{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};
        VkSurfaceKHR surface{};

        VkSwapchainKHR swapChain{};
        std::shared_ptr<SwapChain> oldSwapChain;

        VkFormat format{};
        VkFormat depthFormat{};

        VkExtent2D extent{};
        VkExtent2D windowExtent{};

        std::vector<VkFramebuffer> framebuffers;
        VkRenderPass renderPass{};

        std::vector<std::unique_ptr<Image>> images;
        std::vector<std::unique_ptr<Image>> depthImages;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame{0};
    };

} // namespace lve


#endif //LITTLEVULKANENGINE_SWAPCHAIN_HPP
