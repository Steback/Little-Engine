#ifndef LITTLEENGINE_SWAPCHAIN_HPP
#define LITTLEENGINE_SWAPCHAIN_HPP


#include "vulkan/vulkan.hpp"


class SwapChain {
public:
    struct Details {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

public:
    SwapChain();

    ~SwapChain();

    static Details querySwapChainSupport(vk::PhysicalDevice& device, vk::SurfaceKHR surface);

private:

};


#endif //LITTLEENGINE_SWAPCHAIN_HPP
