#include <iostream>
#include <exception>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "traipse/core/core.h"
#include "vk_enum_string_helper.h"

using namespace std;
using namespace traipse::core;

void cleanup(
        const VkInstance &instance,
        const VkSurfaceKHR surface,
        const VkDevice &device, 
        const VkCommandPool &commandPool, 
        const vector<VkCommandBuffer> commandBuffers,
        GLFWwindow *window
) {

    if (device != VK_NULL_HANDLE) {
        if (commandBuffers.size() > 0) {
            cout << "freeing command buffers" << endl;
            vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());
        }

        if (commandPool != VK_NULL_HANDLE) {
            cout << "destroying command pool" << endl;
            vkDestroyCommandPool(device, commandPool, NULL);
        }

        cout << "destroying device" << endl;
        vkDestroyDevice(device, NULL);
    } 

    if (instance != VK_NULL_HANDLE) {
        if (surface != VK_NULL_HANDLE) {
            cout << "destroying surface" << endl;
            vkDestroySurfaceKHR(instance, surface, NULL);
        }
        cout << "destroying instance" << endl;
        vkDestroyInstance(instance, NULL);
    }

    if (window != NULL) {
        cout << "destroying window" << endl;
        glfwDestroyWindow(window);
    }

    cout << "terminating glfw" << endl;
    glfwTerminate();
}

bool hasFlag(uint32_t flags, uint32_t flag) {
    return (flags & flag) == flag;
}

void printPhysicalDevicesInfo(vector<PhysicalDeviceInfo> physicalDevicesInfo) {
    for (auto info : physicalDevicesInfo) {
        auto deviceName = info.physicalDeviceProperties.deviceName;
        auto deviceType = string_VkPhysicalDeviceType(info.physicalDeviceProperties.deviceType);
        cout << "- " << deviceName << endl;
        cout << " - type: " << deviceType << endl;

        cout << " - extensions: " << endl;
        for (auto extension : info.extensionNames) {
            cout << "  - " << extension << endl;
        }

        cout << " - queue families: " << endl;

        for (size_t idx = 0; idx < info.queueFamilyProperties.size(); ++idx) {
            auto flags = info.queueFamilyProperties.at(idx).queueFlags;
            cout << "   - " << idx << ". flags=" << flags
                << " (graphics=" << hasFlag(flags, VK_QUEUE_GRAPHICS_BIT)
                << ", compute=" << hasFlag(flags, VK_QUEUE_COMPUTE_BIT)
                << ", transfer=" << hasFlag(flags, VK_QUEUE_TRANSFER_BIT)
                << ", sparse=" << hasFlag(flags, VK_QUEUE_SPARSE_BINDING_BIT)
                << ")" << endl;

        }
    }
}

int main(/* int argc, char** argv */) {

    InstanceInfo instanceInfo = {};
    VkDevice device = {};
    VkCommandPool commandPool = {};
    vector<VkCommandBuffer> commandBuffers = {};
    GLFWwindow *window;
    VkSurfaceKHR surface = {};

    VkResult result;
    
    try {

        cout << "initializing glfw and checking support" << endl;
        glfwInit();
        if (!glfwVulkanSupported()) throw std::runtime_error("glfw vulkan is not supported"); 
        
        cout << "creating instance" << endl;
        instanceInfo = createInstance();
        cout << "- extensions: " << endl;
        for (auto extensionName : instanceInfo.extensionNames) {
            cout << " - " << extensionName << endl;
        }

        cout << "creating window" << endl;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(640, 480, "Traipse - Basic", NULL, NULL);

        cout << "creating surface" << endl;
        result = glfwCreateWindowSurface(instanceInfo.instance, window, NULL, &surface);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create surface: " + toMessage(result));
        }



        PhysicalDeviceInfo physicalDeviceInfo;
        uint32_t queueFamilyIndex;
        {
            cout << "acquiring physical devices information" << endl;
            vector<PhysicalDeviceInfo> physicalDevicesInfo = acquirePhysicalDevicesInfo(instanceInfo.instance);
            printPhysicalDevicesInfo(physicalDevicesInfo);

            const auto& [left, right] = selectPhysicalDeviceForPresentation(instanceInfo, physicalDevicesInfo);

            physicalDeviceInfo = physicalDevicesInfo.at(left);
            queueFamilyIndex = right;

            cout
                << "chose physical device: " << physicalDeviceInfo.physicalDeviceProperties.deviceName 
                << ", queue family index: " << right 
                << endl;
        }

        cout << "creating device" << endl;
        device = createDevice(physicalDeviceInfo, queueFamilyIndex);

        cout << "creating command pool" << endl;
        commandPool = createCommandPool(device, queueFamilyIndex);

        cout << "allocating command buffers" << endl;
        commandBuffers = allocateCommandBuffers(device, commandPool, 1);

    } catch (const std::exception &exception) {
        cerr << "error: " << exception.what() << endl;
    }

    cleanup(instanceInfo.instance, surface, device, commandPool, commandBuffers, window);

    return 0;
}
