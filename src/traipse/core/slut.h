#ifndef TRAIPSE_CORE_SLUT_H_
#define TRAIPSE_CORE_SLUT_H_

#include <stdexcept>
#include <string>

#include <vector>

#include <vulkan/vulkan.h>

using std::string, std::vector;

namespace traipse {
namespace core {

string toMessage(const VkResult &result);
vector<char> readBinaryFile(string filePath);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_SLUT_H_
