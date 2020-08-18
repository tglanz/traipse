#include "traipse/core/instances.h"
#include "traipse/core/slut.h"

#include <stdexcept>
#include <GLFW/glfw3.h>

using std::vector;

namespace traipse {
namespace core {

const vector<const char *> DEBUG_LAYERS = { "VK_LAYER_KHRONOS_validation" };

void registerValidationLayers(VkInstanceCreateInfo *instanceCreateInfo) {
    vector<VkLayerProperties> availableLayerProperties = getInstanceLayerProperties();

    for (const char *debugLayer : DEBUG_LAYERS) {
        bool found = false;
        string debugLayerString = debugLayer;

        for (const auto& layerProperties : availableLayerProperties) {
            string layerNameString = layerProperties.layerName;

            if (debugLayerString == layerNameString) {
                found = true;
                break;
            }
        }

        if (!found) throw std::runtime_error(
                "debug layer '" + debugLayerString + "' isn't available");
    }


    instanceCreateInfo->enabledLayerCount = DEBUG_LAYERS.size();
    instanceCreateInfo->ppEnabledLayerNames = DEBUG_LAYERS.data();
}

InstanceInfo createInstance(bool enableValidationLayers) { 

    uint32_t count;
    const char **extensionNames = glfwGetRequiredInstanceExtensions(&count); 

    InstanceInfo ans = {
        .instance = VK_NULL_HANDLE,
        .extensionNames = vector(extensionNames, extensionNames + count)
    };

    VkApplicationInfo applicationInfo;
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = NULL;
    applicationInfo.pApplicationName = "vulkan application";
    applicationInfo.applicationVersion = 1;
    applicationInfo.pEngineName = "vulkan engine";
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = NULL;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = ans.extensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = ans.extensionNames.size() > 0
        ? ans.extensionNames.data()
        : NULL;

    if (enableValidationLayers) {
        registerValidationLayers(&instanceCreateInfo);
    } else {
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = NULL;
    }  

    auto result = vkCreateInstance(&instanceCreateInfo, NULL, &ans.instance);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create instance: " + toMessage(result));

    return ans;
}

vector<VkLayerProperties> getInstanceLayerProperties() {
    uint32_t count;

    VkResult result = vkEnumerateInstanceLayerProperties(&count, NULL);
    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get instance layer properties count: " + toMessage(result));

    vector<VkLayerProperties> ans(count);
    result = vkEnumerateInstanceLayerProperties(&count, ans.data());
    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to get instance layer properties: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
