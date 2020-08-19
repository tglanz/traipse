#ifndef TRAIPSE_CORE_PIPELINE_H_
#define TRAIPSE_CORE_PIPELINE_H_

#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "traipse/core/swapchains.h"
#include "traipse/core/slut.h"

namespace traipse {
namespace core {

struct ShaderModules {
    VkShaderModule vertex;
    VkShaderModule fragment;
};

struct PipelineInfo {
    VkPipeline pipeline;
    ShaderModules shaderModules;
    VkPipelineLayout layout;
    VkRenderPass renderPass;

    VkViewport viewport;
    VkRect2D scissor;
    VkSampleCountFlagBits sampleCount;
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
};

PipelineInfo createPipeline(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo);

VkShaderModule createShaderModuleFromSpirvFile(
    const VkDevice &device,
    std::string filePath);

VkPipelineLayout createPipelineLayout(
    const VkDevice &device);

VkRenderPass createRenderPass(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo,
    VkSampleCountFlagBits sampleCount);

}  // core
}  // namespace traipse

#endif  // TRAIPSE_CORE_PIPELINE_H_
