#include "traipse/core/graphicspipeline.h"
#include "traipse/core/slut.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <stdexcept>

using std::vector, std::string;

namespace traipse {
namespace core {

GraphicsPipelineInfo createGraphicsPipeline(
    const VkDevice &device
) {

    // this will be parameterizd once
    ShaderModules shaderModules = {
        .vertex = createShaderModuleFromSpirvFile(
            device, "shaders/vert.spv"),
        .fragment = createShaderModuleFromSpirvFile(
            device, "shaders/frag.spv")
    }; 

    vector<VkPipelineShaderStageCreateInfo> stages(2);

    // vertex shader stage
    stages.at(0).sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages.at(0).stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages.at(0).module = shaderModules.vertex;
    stages.at(0).pName = "main";

    // fragment shader stage
    stages.at(1).sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages.at(1).stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages.at(1).module = shaderModules.fragment;
    stages.at(1).pName = "main";

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.stageCount = stages.size();
    graphicsPipelineCreateInfo.pStages = stages.data();

    VkPipeline pipeline;
    VkResult result = vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, NULL, &pipeline);

    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create graphics pipeline");

    return {
        .pipeline = pipeline,
        .shaderModules = shaderModules,
    };
}

VkShaderModule createShaderModuleFromSpirvFile(
        const VkDevice &device,
        string filePath) {
    vector<char> code = readBinaryFile(filePath);
    
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = code.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule ans;
    VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, &ans);
    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create shader module from spirv file: " + filePath);

    return ans;
}

}  // core
}  // traipse
