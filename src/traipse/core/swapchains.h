#ifndef TRAIPSE_CORE_SWAPCHAINS_H_
#define TRAIPSE_CORE_SWAPCHAINS_H_

#include <vulkan/vulkan.h>

namespace traipse {
namespace core {
       
VkSwapchainKHR createSwapchain(const VkDevice &device);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SWAPCHAINS_H_
