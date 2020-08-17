#include "traipse/core/surfaces.h"
#include "traipse/core/slut.h"

#include <stdexcept>

using std::vector;

namespace traipse {
namespace core {
    
PhysicalDeviceSurfaceInfo getPhysicalDeviceSurfaceInfo(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) {
    return {
        .physicalDevice = physicalDevice,
        .surface = surface,
        .capabilities = getPhysicalDeviceSurfaceCapabilities(
                physicalDevice, surface),
        .presentModes = getPhysicalDeviceSurfacePresentModes(
                physicalDevice, surface)
    };
}

VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(
        const VkPhysicalDevice &physicalDevice, 
        const VkSurfaceKHR &surface) {
    VkSurfaceCapabilitiesKHR ans = {};
    
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevice, surface, &ans);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface capabilities: " + toMessage(result));

    return ans;
}

vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) {
    vector<VkPresentModeKHR> ans = {};
    
    uint32_t count;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, &count, NULL);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface present modes count: " + toMessage(result));

    ans.resize(count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, &count, ans.data());

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface present modes: " + toMessage(result));

    return ans;

}


}  // core
}  // traipse
