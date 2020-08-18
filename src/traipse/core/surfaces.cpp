#include "traipse/core/surfaces.h"
#include "traipse/core/slut.h"

#include <stdexcept>

using std::vector;

namespace traipse {
namespace core {
    
PhysicalDeviceSurfaceInfo getPhysicalDeviceSurfaceInfo(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) {
    PhysicalDeviceSurfaceInfo ans = {};
    ans.physicalDevice = physicalDevice;
    ans.surface = surface;
    ans.capabilities = getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface);
    ans.presentModes = getPhysicalDeviceSurfacePresentModes(physicalDevice, surface);
    ans.surfaceFormats = getPhysicalDeviceSurfaceFormats(physicalDevice, surface);
    return ans;
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
    
    uint32_t count;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, &count, NULL);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface present modes count: " + toMessage(result));

    vector<VkPresentModeKHR> ans(count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, &count, ans.data());

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface present modes: " + toMessage(result));

    return ans;

}

vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) {

    uint32_t count;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice, surface, &count, NULL);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface format count");

    vector<VkSurfaceFormatKHR> ans(count);

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice, surface, &count, ans.data());

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get physical device surface format count");

    return ans;
}

}  // core
}  // traipse
