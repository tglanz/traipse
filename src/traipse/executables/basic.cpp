#include <vulkan/vulkan.h>
#include <iostream>
#include <exception>
#include <string>
#include <variant>

#include "traipse/core/core.h"
#include "vk_enum_string_helper.h"

using namespace std;
using namespace traipse::core;

void cleanup(const VkInstance &instance) {
    cout << "destroying instance" << endl;
    vkDestroyInstance(instance, NULL);
}

int main(int argc, char** argv) {

    VkInstance instance;
    
    try {
        cout << "creating instance" << endl;
        instance = createInstance();

        cout << "acquiring physical devices" << endl;
        vector<VkPhysicalDevice> physicalDevices = acquirePhysicalDevices(instance);

        vector<VkPhysicalDeviceProperties> physicalDevicesProperties;

        for (auto physicalDevice : physicalDevices) {
            auto physicalDeviceProperties = acquirePhysicalDeviceProperties(physicalDevice);
            physicalDevicesProperties.push_back(physicalDeviceProperties);

            string deviceName = physicalDeviceProperties.deviceName;
            string deviceType = string_VkPhysicalDeviceType(physicalDeviceProperties.deviceType);
            cout << " - name=\"" << deviceName << "\", type=\"" << deviceType << "\"" << endl; 
         } 

        vector<VkQueueFamilyProperties> queueFamilyProperties;

        for (auto physicalDevice : physicalDevices) {
            queueFamilyProperties = acquireDeviceQueueFamilyProperties(physicalDevice);
            if (queueFamilyProperties.size() > 0) {
                break;
            }
        }

    } catch (const std::exception &exception) {
        cerr << "error: " << exception.what() << endl;
    }

    cleanup(instance);

    return 0;
}
