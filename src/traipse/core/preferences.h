#ifndef TRAIPSE_CORE_PREFERENCES_H_
#define TRAIPSE_CORE_PREFERENCES_H_

#include <vulkan/vulkan.h>
#include <vector>

using std::vector;

namespace traipse {
namespace core {

VkSurfaceFormatKHR chooseSurfaceFormat(vector<VkSurfaceFormatKHR> surfaceFormats);
VkPresentModeKHR choosePresentMode(vector<VkPresentModeKHR> presentModes);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_PREFERENCES_H_
