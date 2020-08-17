#ifndef TRAIPSE_CORE_DEVICES_H_
#define TRAIPSE_CORE_DEVICES_H_

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <tuple>
#include <optional>

#include "traipse/core/instances.h"

using std::vector, std::string, std::tuple, std::optional;

namespace traipse {
namespace core {

struct QueueFamilyIndices {
    vector<uint32_t> allGraphicsQueueFamilyIndices;
    vector<uint32_t> allPresentationQueueFamilyIndices;

    optional<uint32_t> graphicsQueueFamilyIndex;
    optional<uint32_t> presentationQueueFamilyIndex;

    bool isCompletelyValid() {
        return graphicsQueueFamilyIndex.has_value() && presentationQueueFamilyIndex.has_value();
    }

    bool isIdeal() {
        return 
            isCompletelyValid() &&
            (graphicsQueueFamilyIndex == presentationQueueFamilyIndex);
    }
};

struct PhysicalDeviceInfo {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    vector<VkQueueFamilyProperties> queueFamilyProperties;
    vector<const char *> extensionNames;
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

VkDevice createDevice(
        const PhysicalDeviceInfo &physicalDeviceInfo,
        const QueueFamilyIndices &queueFamilyIndices);

QueueFamilyIndices discoverQueueFamilyIndices(
        const PhysicalDeviceInfo &physicalDeviceInfo,
        const VkSurfaceKHR &surface);

tuple<size_t, QueueFamilyIndices> selectPhysicalDevice(
        const vector<PhysicalDeviceInfo> &physicalDevicesInfo,
        const VkSurfaceKHR &surface);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_DEVICES_H_
