#ifndef TRAIPSE_CORE_COMMANDS_H_
#define TRAIPSE_CORE_COMMANDS_H_

#include <vector>

#include <vulkan/vulkan.h>

using std::vector;

namespace traipse {
namespace core {

VkCommandPool createCommandPool(const VkDevice &device, uint32_t queueFamilyIndex);

vector<VkCommandBuffer> allocateCommandBuffers(
        const VkDevice &device, const VkCommandPool &commandPool, uint32_t count);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_COMMANDS_H_
