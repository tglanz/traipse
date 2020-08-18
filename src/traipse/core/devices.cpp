/**
 * related material
 * - https://vulkan.lunarg.com/doc/view/1.2.135.0/linux/tutorial/html/03-init_device.html
 */
#include "traipse/core/devices.h"
#include "traipse/core/slut.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>
#include <set>

using std::string, std::vector, std::set;

namespace traipse {
namespace core {

set<string> PhysicalDeviceInfo::getUnsupportedExtensionNames() const {
    set<string> available;
    for (const auto& props : availableExtensionProperties) {
        available.insert(props.extensionName);
    }
    
    set<string> unsupported;
    for (const auto& enabled : enabledExtensionNames) {
        if (!available.contains(enabled)) {
            unsupported.insert(enabled);
        }
    }

    return unsupported;
}

vector<PhysicalDeviceInfo> acquirePhysicalDevicesInfo(
        const VkInstance &instance) {
    vector<PhysicalDeviceInfo> ans;
    
    for (VkPhysicalDevice physicalDevice : acquirePhysicalDevices(instance)) {
        PhysicalDeviceInfo info = {
            .physicalDevice = physicalDevice,
            .physicalDeviceProperties = acquirePhysicalDeviceProperties(physicalDevice),
            .physicalDeviceMemoryProperties = acquirePhysicalDeviceMemoryProperties(physicalDevice),
            .queueFamilyProperties = acquirePhysicalDeviceQueueFamilyProperties(physicalDevice),
            .availableExtensionProperties = getPhysicalDeviceAvailableExtensionProperties(physicalDevice),
            .enabledExtensionNames = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            }
        };

        ans.push_back(info);
    }


    return ans;
}

vector<VkPhysicalDevice> acquirePhysicalDevices(const VkInstance &instance) {

    uint32_t count;
    VkResult result = vkEnumeratePhysicalDevices(instance, &count, nullptr);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to acquire physical devices count: " + toMessage(result));

    if (count > 0) {
        vector<VkPhysicalDevice> ans(count);
        result = vkEnumeratePhysicalDevices(instance, &count, ans.data());

        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to acquire physical devices: " + toMessage(result));
        return ans;
    }

    return {};
}

vector<VkQueueFamilyProperties> acquirePhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice &physicalDevice) {
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, NULL);

    vector<VkQueueFamilyProperties> ans(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, ans.data());

    return ans;
}

VkPhysicalDeviceProperties acquirePhysicalDeviceProperties(const VkPhysicalDevice &physicalDevice) {
    VkPhysicalDeviceProperties ans;
    vkGetPhysicalDeviceProperties(physicalDevice, &ans);
    return ans;
}

VkPhysicalDeviceMemoryProperties acquirePhysicalDeviceMemoryProperties(const VkPhysicalDevice &physicalDevice) {
    VkPhysicalDeviceMemoryProperties ans;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &ans);
    return ans;
}
vector<VkExtensionProperties> getPhysicalDeviceAvailableExtensionProperties(
        const VkPhysicalDevice &physicalDevice) {
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &count, NULL);

    vector<VkExtensionProperties> ans(count);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, ans.data());
    return ans;
}

QueueFamilyIndices discoverQueueFamilyIndices(
        const PhysicalDeviceInfo &physicalDeviceInfo,
        const VkSurfaceKHR &surface) {

    QueueFamilyIndices queueFamilyIndices;
    for (size_t index = 0; index < physicalDeviceInfo.queueFamilyProperties.size(); ++index) {
        auto queueFamilyProperties = physicalDeviceInfo.queueFamilyProperties.at(index);
        auto queueFlags = queueFamilyProperties.queueFlags;

        if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilyIndices.allGraphicsQueueFamilyIndices.push_back(index);
        }

        VkBool32 presentationSupported = false;
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(
                physicalDeviceInfo.physicalDevice, index, surface, &presentationSupported);

        if (result != VK_SUCCESS) throw std::runtime_error(
                "failed to query presentation support");

        if (presentationSupported) {
            queueFamilyIndices.allPresentationQueueFamilyIndices.push_back(index);
        }
    }

    for (uint32_t graphicsIndex : queueFamilyIndices.allGraphicsQueueFamilyIndices) {
        queueFamilyIndices.graphicsQueueFamilyIndex = graphicsIndex;

        for (uint32_t presentationIndex : queueFamilyIndices.allPresentationQueueFamilyIndices) {
            queueFamilyIndices.presentationQueueFamilyIndex = presentationIndex;

            if (graphicsIndex == presentationIndex) {
                return queueFamilyIndices;
            }
        }
    }

    return queueFamilyIndices;
}

VkDevice createDevice(
        const PhysicalDeviceInfo &physicalDeviceInfo, 
        const QueueFamilyIndices &queueFamilyIndices) {
    // pick the first queue that support graphics, use it.
    // TODO: acquire all such queues and use the priority mechanism.

    VkDevice ans = {};

    float queuePriorities = 0;
    VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
    deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsQueueFamilyIndex.value();
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = NULL;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = &queuePriorities;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = physicalDeviceInfo.enabledExtensionNames.size();
    deviceCreateInfo.ppEnabledExtensionNames = physicalDeviceInfo.enabledExtensionNames.size() > 0
        ? physicalDeviceInfo.enabledExtensionNames.data()
        : NULL;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.ppEnabledLayerNames = NULL;
    deviceCreateInfo.pEnabledFeatures = NULL;

    VkResult result = vkCreateDevice(
            physicalDeviceInfo.physicalDevice, &deviceCreateInfo, NULL, &ans);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create device: " + toMessage(result));

    return ans;
}

tuple<size_t, QueueFamilyIndices> selectPhysicalDevice(
        const vector<PhysicalDeviceInfo> &physicalDevicesInfo,
        const VkSurfaceKHR &surface) {

    if (physicalDevicesInfo.size() == 0) throw std::runtime_error(
            "no physical devices provided");

    bool foundCandidate = false;
    size_t candidatePhysicalDeviceIndex;
    QueueFamilyIndices candidateQueueFamilyIndices;

    for (size_t physicalDeviceIndex = 0; physicalDeviceIndex < physicalDevicesInfo.size(); ++physicalDeviceIndex) {
        const auto& physicalDeviceInfo = physicalDevicesInfo.at(physicalDeviceIndex);

        if (physicalDeviceInfo.getUnsupportedExtensionNames().size() > 0) {
            continue;
        }

        QueueFamilyIndices queueFamilyIndices = discoverQueueFamilyIndices(
                physicalDeviceInfo, surface);

        if (queueFamilyIndices.isCompletelyValid()) {
            foundCandidate = true;
            candidatePhysicalDeviceIndex = physicalDeviceIndex;
            candidateQueueFamilyIndices = queueFamilyIndices;

            if (queueFamilyIndices.isIdeal()) {
                break;
            }
        }
    }

    if (!foundCandidate) throw std::runtime_error(
            "failed to find an appropriate physical device");

    return std::make_tuple(candidatePhysicalDeviceIndex, candidateQueueFamilyIndices);
}

}  // core
}  // traipse
