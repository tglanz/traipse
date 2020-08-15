#ifndef TRAIPSE_DEVICES_H_
#define TRAIPSE_DEVICES_H_

#include <vulkan/vulkan.h>

#include <vector>
#include <variant>

namespace traipse {
namespace core {

std::variant<VkResult, vector<VkPhysicalDevice>>
acquirePhysicalDevices(const VkInstance &vkInstance) {
    vector<VkPhysicalDevice> ans;
    uint32_t count;
    VkResult result = vkEnumeratePhysicalDevices(vkInstance, &count, NULL);

    if (result != VK_SUCCESS || count == 0) return result;

    ans.resize(count);
    result = vkEnumeratePhysicalDevices(vkInstance, &count, ans.data());
    if (result != VK_SUCCESS || count == 0) return result;

    return ans;
} 

}  // namespace core
}  // traipe

#endif  // TRAIPSE_DEVICES_H_
