#ifndef TRAIPSE_CORE_QUEUES_H_
#define TRAIPSE_CORE_QUEUES_H_

#include <vulkan/vulkan.h>

#include <vector>
#include <optional>

using std::vector, std::optional;

namespace traipse {
namespace core {

struct QueueFamilyIndices {
    vector<uint32_t> allGraphicsQueueFamilyIndices;
    vector<uint32_t> allPresentationQueueFamilyIndices;

    optional<uint32_t> graphicsQueueFamilyIndex;
    optional<uint32_t> presentationQueueFamilyIndex;

    bool isCompletelyValid() const;
    bool isIdeal() const;
};

struct Queues {
    vector<VkQueue> graphics;
    vector<VkQueue> presentation;
};

Queues getDeviceQueues(
    const VkDevice &device,
    const QueueFamilyIndices &queueFamilyIndices);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_QUEUES_H_
