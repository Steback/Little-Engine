#include "SwapChain.hpp"


SwapChain::SwapChain() = default;

SwapChain::~SwapChain() = default;

SwapChain::Details SwapChain::querySwapChainSupport(vk::PhysicalDevice &device, vk::SurfaceKHR surface) {
    SwapChain::Details details;
    details.capabilities = device.getSurfaceCapabilitiesKHR(surface);

    std::vector<vk::SurfaceFormatKHR> formats = device.getSurfaceFormatsKHR(surface);
    if (!formats.empty())
        details.formats = formats;

    std::vector<vk::PresentModeKHR> presentModes = device.getSurfacePresentModesKHR(surface);
    if (!presentModes.empty())
        details.presentModes = presentModes;

    return details;
}
