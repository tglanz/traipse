#ifndef TRAIPSE_CORE_DEVICES_H_
#define TRAIPSE_CORE_DEVICES_H_

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <tuple>

#include "traipse/core/instances.h"

using std::vector, std::string, std::tuple;

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


tuple<size_t, size_t> selectPhysicalDeviceForPresentation(const InstanceInfo &instanceInfo, const vector<PhysicalDeviceInfo> &physicalDevicesInfo);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_DEVICES_H_
