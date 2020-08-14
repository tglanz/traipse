#include <iostream>

#include <string>
#include <vulkan/vulkan.h>

using namespace std;

constexpr auto createApplicationInfo() {
    VkApplicationInfo ans = {};
    ans.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    ans.pNext = NULL;
    ans.pApplicationName = "vulkan application";
    ans.applicationVersion = 1;
    ans.pEngineName = "vulkan engine";
    ans.apiVersion = VK_API_VERSION_1_0;
    return ans;
}

constexpr auto createInstanceCreateInfo(const VkApplicationInfo *applicationInfo) {
    VkInstanceCreateInfo ans = {};
    ans.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ans.pNext = NULL;
    ans.flags = 0;
    ans.pApplicationInfo = applicationInfo;
    ans.enabledExtensionCount = 0;
    ans.ppEnabledExtensionNames = NULL;
    ans.enabledLayerCount = 0;
    ans.ppEnabledLayerNames = NULL;
    return ans;
}

constexpr auto toMessage(const VkResult &result) {
    switch (result) {
         case VK_SUCCESS: return "Command completed successfully";
         case VK_NOT_READY: return "A fence or query has not yet completed";
         case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
         case VK_EVENT_SET: return "An event is signaled";
         case VK_EVENT_RESET: return "An event is unsignaled";
         case VK_INCOMPLETE: return "A return array was too small for the result";
         case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
         case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
         case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of a object has failed";
         case VK_ERROR_DEVICE_LOST: return "The logical device has been lost. See &lt;&lt;devsandqueues-lost-device&gt;&gt;";
         case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
         case VK_ERROR_LAYER_NOT_PRESENT: return "Layer specified does not exist";
         case VK_ERROR_EXTENSION_NOT_PRESENT: return "Extension specified does not exist";
         case VK_ERROR_FEATURE_NOT_PRESENT: return "Requested feature is not available on this device";
         case VK_ERROR_INCOMPATIBLE_DRIVER: return "Unable to find a Vulkan driver";
         case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
         case VK_ERROR_FORMAT_NOT_SUPPORTED: return "Requested format is not supported on this device";
         case VK_ERROR_FRAGMENTED_POOL: return "A requested pool allocation has failed due to fragmentation of the pool's memory";
         case VK_ERROR_UNKNOWN: return "An unknown error has occurred, due to an implementation or application bug";
         default: return "Unknown enum value";
    }
}

int main(int argc, char** argv) {
    const auto applicationInfo = createApplicationInfo();
    const auto instanceCreateInfo = createInstanceCreateInfo(&applicationInfo);

    VkResult result;
    VkInstance instance;

    result = vkCreateInstance(&instanceCreateInfo, NULL, &instance); 
    cout << "create instance result: " << toMessage(result) << endl; 

    if (result != VK_SUCCESS) {
        return 1;
    }

    vkDestroyInstance(instance, NULL);

    return 0;
}


