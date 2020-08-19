#include "traipse/core/graphicspipeline.h"
#include "traipse/core/slut.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <stdexcept>

using std::vector, std::string;

namespace traipse {
namespace core {

static vector<VkPipelineShaderStageCreateInfo> createShaderStageCreateInfos(
    const ShaderModules &shaderModules
) {
    vector<VkPipelineShaderStageCreateInfo> ans(2);

    // vertex shader stage
    ans.at(0).sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ans.at(0).stage = VK_SHADER_STAGE_VERTEX_BIT;
    ans.at(0).module = shaderModules.vertex;
    ans.at(0).pName = "main";

    // fragment shader stage
    ans.at(1).sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ans.at(1).stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    ans.at(1).module = shaderModules.fragment;
    ans.at(1).pName = "main";

    return ans;
}

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

    vector<VkPipelineShaderStageCreateInfo> shaderStages = createShaderStageCreateInfos(shaderModules);

    // we still don't send any vertices, once we do we need to set the descriptors and stuff
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = VK_NULL_HANDLE,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = VK_NULL_HANDLE,
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = false,
    };

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
    graphicsPipelineCreateInfo.stageCount = shaderStages.size();
    graphicsPipelineCreateInfo.pStages = shaderStages.data();
    graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;

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
