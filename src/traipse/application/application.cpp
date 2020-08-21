#include "traipse/application/application.h"
#include <spdlog/spdlog.h>

using spdlog::info, spdlog::debug;

using namespace traipse::core;

namespace traipse {
namespace application {

void Application::waitIdle() {
    VkResult result = vkDeviceWaitIdle(device);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to wait for device idle: " + toMessage(result));
} 

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

    debug("getting device queues");
    queues = getDeviceQueues(device, queueFamilyIndices);

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

    for (size_t idx = 0; idx < commandBuffers.size(); ++idx) {
        debug("recording draw commands {}/{}", idx + 1, commandBuffers.size());
        VkCommandBuffer commandBuffer = commandBuffers.at(idx);
        VkFramebuffer framebuffer = framebuffers.at(idx);
        recordDrawCommands(commandBuffer, framebuffer, swapchainInfo, pipelineInfo);
    }

    createSyncObjects(maximumFramesInFlight);
}

void Application::createSyncObjects(size_t count) {
    VkResult result;
    
    imageAvailableSempahores.resize(count);
    renderFinishedSemaphores.resize(count);
    inFlightFences.resize(count);

    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t idx = 0; idx < count; ++idx) {
        debug("creating sync objects {}/{}", idx + 1, count);

        result = vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &imageAvailableSempahores.at(idx));
        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create image available semaphore: " + toMessage(result));

        result = vkCreateSemaphore(device, &semaphoreCreateInfo, NULL, &renderFinishedSemaphores.at(idx));
        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create render finished semaphore: " + toMessage(result));

        result = vkCreateFence(device, &fenceCreateInfo, NULL, &inFlightFences.at(idx));
        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create in flight fence: " + toMessage(result));

    }
}

void Application::render() {
    VkResult result;
    uint32_t imageIndex;

    VkSemaphore imageAvailableSempahore = imageAvailableSempahores.at(currentFrame);
    VkSemaphore renderFinishedSemaphore = renderFinishedSemaphores.at(currentFrame);
    VkFence inFlightFence = inFlightFences.at(currentFrame);

    result = vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to wait for fence: " + toMessage(result));

    result = vkResetFences(device, 1, &inFlightFence);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to reset fence: " + toMessage(result));

    result = vkAcquireNextImageKHR(
        device, swapchainInfo.swapchain, UINT64_MAX, imageAvailableSempahore, NULL, &imageIndex);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to acquire next image index: " + toMessage(result));

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSempahore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers.at(imageIndex);

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = vkQueueSubmit(queues.graphics.at(0), 1, &submitInfo, inFlightFence);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to submit to queue: " + toMessage(result));

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchainInfo.swapchain;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = NULL;

    result = vkQueuePresentKHR(queues.presentation.at(0), &presentInfo);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to queue present: " + toMessage(result));

    currentFrame = (currentFrame + 1) % maximumFramesInFlight;
}

void Application::mainLoop() {

    debug("starting main loop");

    while (!pendingClose && !glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render();
    }

    debug("main loop ended, waiting for device to be idle");
    waitIdle();
}

void Application::cleanup() {
    if (device != VK_NULL_HANDLE) {
        
        for (size_t idx = 0; idx < imageAvailableSempahores.size(); ++idx) {
            debug("destroying sync objects {}/{}",
                idx + 1, imageAvailableSempahores.size());
            vkDestroySemaphore(device, renderFinishedSemaphores.at(idx), NULL);
            vkDestroySemaphore(device, imageAvailableSempahores.at(idx), NULL);
            vkDestroyFence(device, inFlightFences.at(idx), NULL);
        }

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