#ifndef TRAIPSE_CORE_FRAMEBUFFERS_H_
#define TRAIPSE_CORE_FRAMEBUFFERS_H_

#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "traipse/core/slut.h"
#include "traipse/core/swapchains.h"
#include "traipse/core/pipeline.h"

namespace traipse {
namespace core {

vector<VkFramebuffer> createFramebuffers(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo,
    const PipelineInfo &pipelineInfo);

}  // namespace core
}  // namespace traipse

#endif  // TRAIPSE_CORE_FRAMEBUFFERS_H_
