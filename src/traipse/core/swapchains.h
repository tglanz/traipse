#ifndef TRAIPSE_CORE_SWAPCHAINS_H_
#define TRAIPSE_CORE_SWAPCHAINS_H_

#include <vulkan/vulkan.h>

namespace traipse {
namespace core {
       
struct SwapchainInfo {
    VkSwapchainKHR swapchain;
    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D imageExtent;
};

SwapchainInfo createSwapchain(
        const VkDevice &device,
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SWAPCHAINS_H_
