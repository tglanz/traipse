#include "traipse/core/swapchains.h"
#include "traipse/core/surfaces.h"
#include "traipse/core/preferences.h"
#include "traipse/core/slut.h"

#include <stdexcept>

namespace traipse {
namespace core {

VkSwapchainKHR createSwapchain(
        const VkDevice &device,
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) {
    VkSwapchainKHR ans = {};
    VkResult result;

    PhysicalDeviceSurfaceInfo physicalDeviceSurfaceInfo = getPhysicalDeviceSurfaceInfo(
           physicalDevice, surface); 

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = NULL;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.imageExtent = physicalDeviceSurfaceInfo.capabilities.currentExtent;
    if (swapchainCreateInfo.imageExtent.width == 0xffffffff) throw std::runtime_error(
           "unhandled case of when the image extent is not initialized"); 
    swapchainCreateInfo.presentMode = choosePresentMode(physicalDeviceSurfaceInfo.presentModes);

    result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &ans); 
    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create swapchain: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
