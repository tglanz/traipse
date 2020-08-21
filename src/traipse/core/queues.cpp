#include "traipse/core/queues.h"

namespace traipse {
namespace core {

bool QueueFamilyIndices::isCompletelyValid() const {
    return graphicsQueueFamilyIndex.has_value() && presentationQueueFamilyIndex.has_value();
}

bool QueueFamilyIndices::isIdeal() const {
    return 
        isCompletelyValid() &&
        (graphicsQueueFamilyIndex == presentationQueueFamilyIndex);
}

Queues getDeviceQueues(
    const VkDevice &device,
    const QueueFamilyIndices &queueFamilyIndices
) {
    Queues queues = {};

    // for now, we just take the first queue of each family
    queues.graphics.resize(1);
    queues.presentation.resize(1);

    vkGetDeviceQueue(
        device, queueFamilyIndices.graphicsQueueFamilyIndex.value(), 0, &queues.graphics.at(0));

    vkGetDeviceQueue(
        device, queueFamilyIndices.presentationQueueFamilyIndex.value(), 0, &queues.presentation.at(0));

    return queues;
}

}  // namespace core
}  // namespace traipse
