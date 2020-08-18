#ifndef TRAIPSE_CORE_DEVICES_H_
#define TRAIPSE_CORE_DEVICES_H_

#include <vulkan/vulkan.h>

#include <vector>
#include <set>
#include <tuple>
#include <string>

#include "traipse/core/instances.h"
#include "traipse/core/queues.h"

using std::vector, std::tuple, std::set, std::string;

namespace traipse {
namespace core {

struct PhysicalDeviceInfo {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vector<VkQueueFamilyProperties> queueFamilyProperties;
    vector<VkExtensionProperties> availableExtensionProperties;
    vector<const char *> enabledExtensionNames;

    set<string> getUnsupportedExtensionNames() const;
};

vector<PhysicalDeviceInfo> acquirePhysicalDevicesInfo(
        const VkInstance &instance);

vector<VkPhysicalDevice> acquirePhysicalDevices(
        const VkInstance &instance);

vector<VkQueueFamilyProperties> acquirePhysicalDeviceQueueFamilyProperties(
        const VkPhysicalDevice &physicalDevice);

VkPhysicalDeviceProperties acquirePhysicalDeviceProperties(
        const VkPhysicalDevice &physicalDevice);

VkPhysicalDeviceMemoryProperties acquirePhysicalDeviceMemoryProperties(
        const VkPhysicalDevice &physicalDevice);

vector<VkExtensionProperties> getPhysicalDeviceAvailableExtensionProperties(
        const VkPhysicalDevice &physicalDevice);

VkDevice createDevice(
        const PhysicalDeviceInfo &physicalDeviceInfo,
        const QueueFamilyIndices &queueFamilyIndices);

QueueFamilyIndices discoverQueueFamilyIndices(
        const PhysicalDeviceInfo &physicalDevice,
        const VkSurfaceKHR &surface);

tuple<size_t, QueueFamilyIndices> selectPhysicalDevice(
        const vector<PhysicalDeviceInfo> &physicalDevicesInfo,
        const VkSurfaceKHR &surface);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_DEVICES_H_
