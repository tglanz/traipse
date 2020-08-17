#include "traipse/core/swapchains.h"
#include "traipse/core/slut.h"

namespace traipse {
namespace core {

VkSwapchainKHR createSwapchain(const VkDevice &device, const VkSurfaceKHR &surface) {
    VkSwapchainKHR ans = {};

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = NULL;
    swapchainCreateInfo.surface = surface;

    VkResult result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &ans); 
    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create swapchain: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
