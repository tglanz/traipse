#ifndef TRAIPSE_CORE_SLUT_H_
#define TRAIPSE_CORE_SLUT_H_

#include <stdexcept>
#include <vector>
#include <string>

#include <vulkan/vulkan.h>

using std::string, std::vector;

namespace traipse {
namespace core {

string toMessage(const VkResult &result);
VkInstance createInstance();

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SLUT_H_
