#include "traipse/application/application.h"

#include <iostream>
using std::cout, std::endl;

using namespace traipse::core;

namespace traipse {
namespace application {

void Application::initWindow() {
    cout << "initializing glfw and checking support" << endl;
    glfwInit();
    if (!glfwVulkanSupported()) throw std::runtime_error(
        "glfw vulkan is not supported"); 

    cout << "creating window" << endl;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(640, 480, "Traipse - Basic", NULL, NULL);
}

void Application::initVulkan() {

    VkResult result;

    cout << "creating instance" << endl;
    instanceInfo = createInstance(true);

    cout << "creating surface" << endl;
    result = glfwCreateWindowSurface(instanceInfo.instance, window, NULL, &surface);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create surface: " + toMessage(result));

    // acquire a physical device
    // scope this to not gather some garbage variables
    {
        cout << "acquiring physical devices information" << endl;
        vector<PhysicalDeviceInfo> physicalDevicesInfo = acquirePhysicalDevicesInfo(
                instanceInfo.instance);

        cout << "selecting physical device and queue families" << endl;
        const auto& [left, right] = selectPhysicalDevice(
                physicalDevicesInfo, surface);

        physicalDeviceInfo = physicalDevicesInfo.at(left);
        queueFamilyIndices = right;

        cout
            << "- chose physical device: " << physicalDeviceInfo.physicalDeviceProperties.deviceName << endl
            << "- queue family indices completely valid: " << queueFamilyIndices.isCompletelyValid() << endl
            << "- queue family indices ideal: " << queueFamilyIndices.isIdeal() << endl;
    }

    cout << "creating device" << endl;
    device = createDevice(physicalDeviceInfo, queueFamilyIndices);

    cout << "creating a swapchain" << endl;
    swapchainInfo = createSwapchain(device, physicalDeviceInfo.physicalDevice, surface);

    cout << "creating a pipeline" << endl;
    pipelineInfo = createPipeline(device, swapchainInfo);

    cout << "creating framebuffers" << endl;
    framebuffers = createFramebuffers(device, swapchainInfo, pipelineInfo);

    cout << "creating command pool" << endl;
    commandPool = createCommandPool(device, queueFamilyIndices.graphicsQueueFamilyIndex.value());

    cout << "allocating command buffers" << endl;
    commandBuffers = allocateCommandBuffers(device, commandPool, swapchainInfo.imageViews.size());
}

void Application::render() {
    
}

void Application::mainLoop() {
    
}

void Application::cleanup() {
    if (device != VK_NULL_HANDLE) {
        
        if (commandBuffers.size() > 0) {
            cout << "freeing command buffers" << endl;
            vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());
        }

        cout << "destroying command pool" << endl;
        vkDestroyCommandPool(device, commandPool, NULL);

        for (size_t idx = 0; idx < swapchainInfo.imageViews.size(); ++idx) {
            cout << "destroying image view " << idx + 1 << "/" << swapchainInfo.imageViews.size() << endl;
            vkDestroyImageView(device, swapchainInfo.imageViews.at(idx), NULL);
        }

        for (size_t idx = 0; idx < framebuffers.size(); ++idx) {
            cout << "destroying framebuffer " << idx + 1 << "/" << framebuffers.size() << endl;
            vkDestroyFramebuffer(device, framebuffers.at(idx), NULL);
        }

        cout << "destroying swapchain" << endl;
        vkDestroySwapchainKHR(device, swapchainInfo.swapchain, NULL);

        cout << "destroying vertex shader module" << endl;
        vkDestroyShaderModule(device, pipelineInfo.shaderModules.vertex, NULL);

        cout << "destroying fragment shader module" << endl;
        vkDestroyShaderModule(device, pipelineInfo.shaderModules.fragment, NULL);

        cout << "destroying render pass" << endl;
        vkDestroyRenderPass(device, pipelineInfo.renderPass, NULL);

        cout << "destroying pipeline layout" << endl;
        vkDestroyPipelineLayout(device, pipelineInfo.layout, NULL);

        cout << "destroying pipeline" << endl;
        vkDestroyPipeline(device, pipelineInfo.pipeline, NULL);

        cout << "destroying device" << endl;
        vkDestroyDevice(device, NULL);
    } 


    if (instanceInfo.instance != VK_NULL_HANDLE) {
        if (surface != VK_NULL_HANDLE) {
            cout << "destroying surface" << endl;
            vkDestroySurfaceKHR(instanceInfo.instance, surface, NULL);
        }
        cout << "destroying instance" << endl;
        vkDestroyInstance(instanceInfo.instance, NULL);
    }

    if (window != NULL) {
        cout << "destroying window" << endl;
        glfwDestroyWindow(window);
    }

    cout << "terminating glfw" << endl;
    glfwTerminate();   
}

}
}