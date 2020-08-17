#ifndef TRAIPSE_CORE_DEVICES_H_
#define TRAIPSE_CORE_DEVICES_H_

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

using std::vector, std::string;

namespace traipse {
namespace core {

struct PhysicalDeviceInfo {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vector<VkQueueFamilyProperties> queueFamilyProperties;
    vector<const char *> extensionNames;
};

vector<PhysicalDeviceInfo> acquirePhysicalDevicesInfo(const VkInstance &instance);

vector<VkPhysicalDevice> acquirePhysicalDevices(const VkInstance &instance);

vector<VkQueueFamilyProperties> acquirePhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice &physicalDevice);


VkPhysicalDeviceProperties acquirePhysicalDeviceProperties(const VkPhysicalDevice &physicalDevice);
VkPhysicalDeviceMemoryProperties acquirePhysicalDeviceMemoryProperties(const VkPhysicalDevice &physicalDevice);

VkDevice createDevice(const PhysicalDeviceInfo &physicalDeviceInfo, uint32_t queueFamilyIndex);

uint32_t selectGraphicsQueueFamilyIndex(const vector<VkQueueFamilyProperties> &queueFamilyProperties);
}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_DEVICES_H_
