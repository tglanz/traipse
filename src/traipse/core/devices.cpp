#include "traipse/core/devices.h"
#include "traipse/core/slut.h"

#include <stdexcept>
#include <string>

using std::string, std::vector;

namespace traipse {
namespace core {

vector<PhysicalDeviceInfo> acquirePhysicalDevicesInfos(const VkInstance& instance) {
    vector<PhysicalDeviceInfo> ans;
    
    for (VkPhysicalDevice physicalDevice : acquirePhysicalDevices(instance)) {
        ans.push_back({
            .physicalDevice = physicalDevice,
            .physicalDeviceProperties = acquirePhysicalDeviceProperties(physicalDevice),
            .physicalDeviceQueueFamilyProperties = acquirePhysicalDeviceQueueFamilyProperties(physicalDevice)
        });
    }

    return ans;
}

vector<VkPhysicalDevice> acquirePhysicalDevices(const VkInstance &instance) {
    vector<VkPhysicalDevice> ans;

    uint32_t count;
    VkResult result = vkEnumeratePhysicalDevices(instance, &count, nullptr);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to acquire physical devices count: " + toMessage(result));

    if (count > 0) {
        ans.resize(count);
        result = vkEnumeratePhysicalDevices(instance, &count, ans.data());

        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to acquire physical devices: " + toMessage(result));
    }

    return ans;
}

VkPhysicalDeviceProperties acquirePhysicalDeviceProperties(const VkPhysicalDevice &physicalDevice) {
    VkPhysicalDeviceProperties ans;
    vkGetPhysicalDeviceProperties(physicalDevice, &ans);
    return ans;
}

vector<VkQueueFamilyProperties> acquirePhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice &physicalDevice) {
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, NULL);

    vector<VkQueueFamilyProperties> ans;
    ans.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, ans.data());

    return ans;
}

}  // core
}  // traipse
