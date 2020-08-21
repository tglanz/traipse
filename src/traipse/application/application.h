#ifndef TRAIPSE_APPLICATION_APPLICATION_H_
#define TRAIPSE_APPLICATION_APPLICATION_H_

#include <iostream>
#include <exception>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "traipse/core/core.h"

namespace traipse {
namespace application {

class Application {
  private:
    VkSurfaceKHR surface;
    VkDevice device;
    
    traipse::core::InstanceInfo instanceInfo;
    traipse::core::PhysicalDeviceInfo physicalDeviceInfo;
    traipse::core::QueueFamilyIndices queueFamilyIndices;
    traipse::core::SwapchainInfo swapchainInfo;
    traipse::core::PipelineInfo pipelineInfo;

    vector<VkFramebuffer> framebuffers;

    VkCommandPool commandPool;
    vector<VkCommandBuffer> commandBuffers;

    GLFWwindow *window;

    void render();

  public:
    void initWindow();
    void initVulkan();
    void cleanup();

    void mainLoop();
};

}  // namespace traipse
}  // namespace application

#endif  // TRAIPSE_APPLICATION_APPLICATION_H_