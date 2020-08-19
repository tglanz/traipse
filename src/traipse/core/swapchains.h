#ifndef TRAIPSE_CORE_SWAPCHAINS_H_
#define TRAIPSE_CORE_SWAPCHAINS_H_

#include <vulkan/vulkan.h>
#include <vector>

using std::vector;

namespace traipse {
namespace core {
       
struct SwapchainInfo {
    VkSwapchainKHR swapchain;
    VkSurfaceFormatKHR surfaceFormat;
    VkExtent2D imageExtent;
    vector<VkImage> images;
    vector<VkImageView> imageViews;
};

SwapchainInfo createSwapchain(
        const VkDevice &device,
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface);

vector<VkImage> getSwapchainImages(
        const VkDevice &device,
        const VkSwapchainKHR &swapchain);

vector<VkImageView> createImageViews(
    const VkDevice &device,
    const vector<VkImage> &images,
    const VkFormat format);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SWAPCHAINS_H_
