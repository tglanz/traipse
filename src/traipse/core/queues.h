#ifndef TRAIPSE_CORE_QUEUES_H_
#define TRAIPSE_CORE_QUEUES_H_

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

#include "traipse/core/devices.h"

using std::vector, std::optional;

namespace traipse {
namespace core {

struct QueueFamilyIndices {
    vector<uint32_t> allGraphicsQueueFamilyIndices;
    vector<uint32_t> allPresentationQueueFamilyIndices;

    optional<uint32_t> graphicsQueueFamilyIndex;
    optional<uint32_t> presentationQueueFamilyIndex;

    constexpr inline bool isCompletelyValid() const {
        return graphicsQueueFamilyIndex.has_value() && presentationQueueFamilyIndex.has_value();
    }

    constexpr inline bool isIdeal() const {
        return 
            isCompletelyValid() &&
            (graphicsQueueFamilyIndex == presentationQueueFamilyIndex);
    }
};

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_QUEUES_H_
