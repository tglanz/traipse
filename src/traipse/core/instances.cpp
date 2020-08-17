#include "traipse/core/instances.h"
#include "traipse/core/slut.h"

#include <stdexcept>

namespace traipse {
namespace core {
    
InstanceInfo createInstance() {

    InstanceInfo ans = {
        .extensionNames = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_KHR_XLIB_SURFACE_EXTENSION_NAME
        }
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
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = NULL;

    auto result = vkCreateInstance(&instanceCreateInfo, NULL, &ans.instance);

    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create instance: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
