#ifndef TRAIPSE_APPLICATION_APPLICATION_H_
#define TRAIPSE_APPLICATION_APPLICATION_H_

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "traipse/core/core.h"

namespace traipse {
namespace application {

class Application {
  private:

    uint32_t currentFrame = 0;
    bool pendingClose = false;

    VkSurfaceKHR surface;
    VkDevice device;
    
    traipse::core::InstanceInfo instanceInfo;
    traipse::core::PhysicalDeviceInfo physicalDeviceInfo;
    traipse::core::QueueFamilyIndices queueFamilyIndices;
    traipse::core::Queues queues;
    traipse::core::SwapchainInfo swapchainInfo;
    traipse::core::PipelineInfo pipelineInfo;

    vector<VkFramebuffer> framebuffers;

    VkCommandPool commandPool;
    vector<VkCommandBuffer> commandBuffers;

    vector<VkSemaphore> imageAvailableSempahores;
    vector<VkSemaphore> renderFinishedSemaphores;
    vector<VkFence> inFlightFences;

    GLFWwindow *window;

    void createSyncObjects(size_t count);
    void render();
    void waitIdle();

  public:
    const uint32_t maximumFramesInFlight;

    Application(uint32_t maximumFramesInFlight)
        : maximumFramesInFlight(maximumFramesInFlight) {};

    constexpr inline void close() { pendingClose = true; }

    void initWindow();
    void initVulkan();
    void cleanup();

    void mainLoop();
};

}  // namespace traipse
}  // namespace application

#endif  // TRAIPSE_APPLICATION_APPLICATION_H_