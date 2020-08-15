#ifndef TRAIPSE_CORE_LAYERS_H_
#define TRAIPSE_CORE_LAYERS_H_

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <variant>

using std::string;
using std::vector;
using std::variant;

namespace traipse {
namespace core {

struct LayerProperties {
    VkLayerProperties properties;
    vector<VkExtensionProperties> extensionsProperties;
};

variant<VkResult, LayerProperties>
acquireLayerProperties(const VkLayerProperties &layerProperties) {
    LayerProperties ans;
    VkResult result;

    do {
        uint32_t count;
        result = vkEnumerateInstanceExtensionProperties(layerProperties.layerName, &count, NULL);

        if (result != VK_SUCCESS) return result;
        if (count == 0) return VK_SUCCESS;

        ans.extensionsProperties.resize(count);
        result = vkEnumerateInstanceExtensionProperties(
                layerProperties.layerName, &count, ans.extensionsProperties.data());
    } while (result == VK_INCOMPLETE);

    return ans;
}

variant<VkResult, vector<LayerProperties>>
acquireLayersProperties() {
    vector<LayerProperties> ans;
    vector<VkLayerProperties> vkProps;
    uint32_t count;
    VkResult result;

    do {
        result = vkEnumerateInstanceLayerProperties(&count, NULL);

        if (result != VK_SUCCESS) return result;
        if (count == 0) return VK_SUCCESS;
   
        // remove this line when it hits and make sure everything is working properly
        throw "this case hasn't been debug yet because it hadn't occurred";

        vkProps.resize(count);
        result = vkEnumerateInstanceLayerProperties(&count, vkProps.data());
    } while (result == VK_INCOMPLETE);

    for (auto idx = 0; idx < count; ++idx) {
        auto response = acquireLayerProperties(vkProps.at(idx));

        if (auto resultPtr = std::get_if<VkResult>(&response)) {
            return *resultPtr;
        }

        ans.push_back(std::get<LayerProperties>(response));
    }

    return ans;
}

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_LAYERS_H_
