#include <iostream>
#include <exception>

#include <vulkan/vulkan.h>

#include "traipse/core/core.h"
#include "vk_enum_string_helper.h"

using namespace std;
using namespace traipse::core;

void cleanup(const VkInstance &instance) {
    cout << "destroying instance" << endl;
    vkDestroyInstance(instance, NULL);
}

bool hasFlag(uint32_t flags, uint32_t flag) {
    return (flags & flag) == flag;
}

void printPhysicalDevicesInfos(vector<PhysicalDeviceInfo> physicalDevicesInfos) {
    for (auto info : physicalDevicesInfos) {
        auto deviceName = info.physicalDeviceProperties.deviceName;
        auto deviceType = string_VkPhysicalDeviceType(info.physicalDeviceProperties.deviceType);
        cout << "- " << deviceName << "; " << deviceType << endl;

        cout << "  - queue families: ";
        for (auto queueFamilyProperties : info.physicalDeviceQueueFamilyProperties) {
            auto flags = queueFamilyProperties.queueFlags;
            cout << "flags=" << flags
                << " (graphics=" << hasFlag(flags, VK_QUEUE_GRAPHICS_BIT)
                << ", compute=" << hasFlag(flags, VK_QUEUE_COMPUTE_BIT)
                << ", transfer=" << hasFlag(flags, VK_QUEUE_TRANSFER_BIT)
                << ", sparse=" << hasFlag(flags, VK_QUEUE_SPARSE_BINDING_BIT)
                << ")" << endl;
        }
    }
}

int main(int argc, char** argv) {

    VkInstance instance;
    
    try {
        cout << "creating instance" << endl;
        instance = createInstance();

        cout << "acquiring physical devices information" << endl;
        vector<PhysicalDeviceInfo> physicalDevicesInfos = acquirePhysicalDevicesInfos(instance);
        printPhysicalDevicesInfos(physicalDevicesInfos);
    } catch (const std::exception &exception) {
        cerr << "error: " << exception.what() << endl;
    }

    cleanup(instance);

    return 0;
}
