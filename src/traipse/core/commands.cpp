#include "traipse/core/commands.h"
#include "traipse/core/slut.h"

using std::string, std::vector;

namespace traipse {
namespace core {

VkCommandPool createCommandPool(const VkDevice &device, uint32_t queueFamilyIndex) {
    VkCommandPool ans = {};

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = NULL;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkResult result = vkCreateCommandPool(device, &commandPoolCreateInfo, NULL, &ans);

    return ans;
}

vector<VkCommandBuffer> allocateCommandBuffers(const VkDevice &device, const VkCommandPool &commandPool, uint32_t count) {
    vector<VkCommandBuffer> ans = {};
    ans.resize(count);

    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = NULL;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = count;

    VkResult result = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, ans.data());

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to allocate command buffers: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
