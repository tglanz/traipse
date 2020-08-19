#ifndef TRAIPSE_CORE_GRAPHICSPIPELINE_H_
#define TRAIPSE_CORE_GRAPHICSPIPELINE_H_

#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>
#include <vector>

#include "traipse/core/slut.h"

namespace traipse {
namespace core {

struct ShaderModules {
    VkShaderModule vertex;
    VkShaderModule fragment;
};

struct GraphicsPipelineInfo {
    VkPipeline pipeline;
    ShaderModules shaderModules;  
};

GraphicsPipelineInfo createGraphicsPipeline(
    const VkDevice &device);

VkShaderModule createShaderModuleFromSpirvFile(
    const VkDevice &device,
    std::string filePath);

}  // core
}  // namespace traipse

#endif  // TRAIPSE_CORE_GRAPHICSPIPELINE_H_
