#ifndef TRAIPSE_CORE_DEVICES_H_
#define TRAIPSE_CORE_DEVICES_H_

#include <vulkan/vulkan.h>
#include <vector>

using std::vector;

namespace traipse {
namespace core {

struct PhysicalDeviceInfo {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vector<VkQueueFamilyProperties> physicalDeviceQueueFamilyProperties;
};

vector<PhysicalDeviceInfo> acquirePhysicalDevicesInfos(const VkInstance &instance);

vector<VkPhysicalDevice> acquirePhysicalDevices(const VkInstance &instance);

VkPhysicalDeviceProperties acquirePhysicalDeviceProperties(const VkPhysicalDevice &physicalDevice);

vector<VkQueueFamilyProperties> acquirePhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice &physicalDevice);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_DEVICES_H_
