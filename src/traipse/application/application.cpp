#include "traipse/application/application.h"
#include <spdlog/spdlog.h>

using spdlog::info, spdlog::debug;

using namespace traipse::core;

namespace traipse {
namespace application {

void Application::initWindow() {
    debug("initializing glfw and checking support");
    glfwInit();
    if (!glfwVulkanSupported()) throw std::runtime_error(
        "glfw vulkan is not supported"); 

    debug("creating window");
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(640, 480, "Traipse - Basic", NULL, NULL);
}

void Application::initVulkan() {

    VkResult result;

    debug("creating instance");
    instanceInfo = createInstance(true);

    debug("creating surface");
    result = glfwCreateWindowSurface(instanceInfo.instance, window, NULL, &surface);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create surface: " + toMessage(result));

    // acquire a physical device
    // scope this to not gather some garbage variables
    {
        debug("acquiring physical devices information");
        vector<PhysicalDeviceInfo> physicalDevicesInfo = acquirePhysicalDevicesInfo(
                instanceInfo.instance);

        debug("selecting physical device and queue families");
        const auto& [left, right] = selectPhysicalDevice(
                physicalDevicesInfo, surface);

        physicalDeviceInfo = physicalDevicesInfo.at(left);
        queueFamilyIndices = right;
    }

    debug("creating device");
    device = createDevice(physicalDeviceInfo, queueFamilyIndices);

    debug("creating a swapchain");
    swapchainInfo = createSwapchain(device, physicalDeviceInfo.physicalDevice, surface);

    debug("creating a pipeline");
    pipelineInfo = createPipeline(device, swapchainInfo);

    debug("creating framebuffers");
    framebuffers = createFramebuffers(device, swapchainInfo, pipelineInfo);

    debug("creating command pool");
    commandPool = createCommandPool(device, queueFamilyIndices.graphicsQueueFamilyIndex.value());

    debug("allocating command buffers");
    commandBuffers = allocateCommandBuffers(device, commandPool, swapchainInfo.imageViews.size());
}

void Application::render() {
    
}

void Application::mainLoop() {
    
}

void Application::cleanup() {
    if (device != VK_NULL_HANDLE) {
        
        if (commandBuffers.size() > 0) {
            debug("freeing command buffers");
            vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());
        }

        debug("destroying command pool");
        vkDestroyCommandPool(device, commandPool, NULL);

        for (size_t idx = 0; idx < swapchainInfo.imageViews.size(); ++idx) {
            debug("destroying image view {}/{}", idx + 1, swapchainInfo.imageViews.size());
            vkDestroyImageView(device, swapchainInfo.imageViews.at(idx), NULL);
        }

        for (size_t idx = 0; idx < framebuffers.size(); ++idx) {
            debug("destroying framebuffer {}/{}", idx + 1, framebuffers.size());
            vkDestroyFramebuffer(device, framebuffers.at(idx), NULL);
        }

        debug("destroying swapchain");
        vkDestroySwapchainKHR(device, swapchainInfo.swapchain, NULL);

        debug("destroying vertex shader module");
        vkDestroyShaderModule(device, pipelineInfo.shaderModules.vertex, NULL);

        debug("destroying fragment shader module");
        vkDestroyShaderModule(device, pipelineInfo.shaderModules.fragment, NULL);

        debug("destroying render pass");
        vkDestroyRenderPass(device, pipelineInfo.renderPass, NULL);

        debug("destroying pipeline layout");
        vkDestroyPipelineLayout(device, pipelineInfo.layout, NULL);

        debug("destroying pipeline");
        vkDestroyPipeline(device, pipelineInfo.pipeline, NULL);

        debug("destroying device");
        vkDestroyDevice(device, NULL);
    } 


    if (instanceInfo.instance != VK_NULL_HANDLE) {
        if (surface != VK_NULL_HANDLE) {
            debug("destroying surface");
            vkDestroySurfaceKHR(instanceInfo.instance, surface, NULL);
        }
        debug("destroying instance");
        vkDestroyInstance(instanceInfo.instance, NULL);
    }

    if (window != NULL) {
        debug("destroying window");
        glfwDestroyWindow(window);
    }

    debug("terminating glfw");
    glfwTerminate();   
}

}
}