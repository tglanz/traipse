#ifndef TRAIPSE_CORE_INSTANCES_H_
#define TRAIPSE_CORE_INSTANCES_H_

#include <vulkan/vulkan.h>
#include <vector>

using std::vector;

namespace traipse {
namespace core {

struct InstanceInfo {
    VkInstance instance;
    vector<const char *> extensionNames;
};

InstanceInfo createInstance(bool enableDebugLayers);
vector<VkLayerProperties> getInstanceLayerProperties(); 

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_INSTANCES_H_
