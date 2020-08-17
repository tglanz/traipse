#ifndef TRAIPSE_CORE_SURFACES_H_
#define TRAIPSE_CORE_SURFACES_H_

#include <vulkan/vulkan.h>

#include <vector>

using std::vector;

namespace traipse {
namespace core {

struct PhysicalDeviceSurfaceInfo {
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    VkSurfaceCapabilitiesKHR capabilities;
    vector<VkPresentModeKHR> presentModes;
    vector<VkSurfaceFormatKHR> surfaceFormats;
};

PhysicalDeviceSurfaceInfo getPhysicalDeviceSurfaceInfo(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface);

VkSurfaceCapabilitiesKHR getPhysicalDeviceSurfaceCapabilities(
        const VkPhysicalDevice &physicalDevice, 
        const VkSurfaceKHR &surface);

vector<VkPresentModeKHR> getPhysicalDeviceSurfacePresentModes(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface);

vector<VkSurfaceFormatKHR> getPhysicalDeviceSurfaceFormats(
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface);
        
}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SURFACES_H_
