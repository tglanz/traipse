#include "traipse/core/slut.h"

#include <string>
#include <vector>
#include <fstream>

using std::string, std::vector;

namespace traipse {
namespace core {

string toMessage(const VkResult &result) {
    switch (result) {
         case VK_SUCCESS:
             return "Command completed successfully";
         case VK_NOT_READY:
             return "A fence or query has not yet completed";
         case VK_TIMEOUT:
             return "A wait operation has not completed in the specified time";
         case VK_EVENT_SET:
             return "An event is signaled";
         case VK_EVENT_RESET:
             return "An event is unsignaled";
         case VK_INCOMPLETE:
             return "A return array was too small for the result";
         case VK_ERROR_OUT_OF_HOST_MEMORY:
             return "A host memory allocation has failed";
         case VK_ERROR_OUT_OF_DEVICE_MEMORY:
             return "A device memory allocation has failed";
         case VK_ERROR_INITIALIZATION_FAILED:
             return "Initialization of a object has failed";
         case VK_ERROR_DEVICE_LOST:
             return "The logical device has been lost";
         case VK_ERROR_MEMORY_MAP_FAILED:
             return "Mapping of a memory object has failed";
         case VK_ERROR_LAYER_NOT_PRESENT:
             return "Layer specified does not exist";
         case VK_ERROR_EXTENSION_NOT_PRESENT:
             return "Extension specified does not exist";
         case VK_ERROR_FEATURE_NOT_PRESENT:
             return "Requested feature is not available on this device";
         case VK_ERROR_INCOMPATIBLE_DRIVER:
             return "Unable to find a Vulkan driver";
         case VK_ERROR_TOO_MANY_OBJECTS:
             return "Too many objects of the type have already been created";
         case VK_ERROR_FORMAT_NOT_SUPPORTED:
             return "Requested format is not supported on this device";
         case VK_ERROR_FRAGMENTED_POOL:
             return "A requested pool allocation has failed due to fragmentation of the pool's memory";
         case VK_ERROR_UNKNOWN:
             return "An unknown error has occurred, due to an implementation or application bug";
         default:
             return "Unknown enum value";
    }
}

vector<char> readBinaryFile(string filePath) {
    // ate - start at end so we can know the file size and initialize the vector
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error(
        "failed to open file: " + filePath);

    size_t fileSize = file.tellg();
    vector<char> ans(fileSize);

    file.seekg(0, std::ios::beg);
    file.read(ans.data(), fileSize);

    return ans;
}

}  // namespace core
}  // namespace traipse