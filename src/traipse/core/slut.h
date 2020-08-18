#ifndef TRAIPSE_CORE_SLUT_H_
#define TRAIPSE_CORE_SLUT_H_

#include <stdexcept>
#include <vector>
#include <set>
#include <string>

#include <vulkan/vulkan.h>

using std::string, std::vector, std::set;

namespace traipse {
namespace core {

string toMessage(const VkResult &result);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SLUT_H_
