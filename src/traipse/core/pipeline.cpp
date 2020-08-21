#include "traipse/core/pipeline.h"
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

/**
 * @param viewport mutated and used as a pointer member in the return value as pViewports.
 * required due to lifetime constraints.
 * 
 * @param scissor mutated and used as a pointer member in the return value
 * as pScissors. required due to lifetime constraints.
 */
static VkPipelineViewportStateCreateInfo createViewportStateCreateInfo(
    const SwapchainInfo &swapchainInfo,
    VkViewport *viewport,
    VkRect2D *scissor
) {
    viewport->x = 0.0f;
    viewport->y = 0.0f;
    viewport->width = static_cast<float>(swapchainInfo.imageExtent.width);
    viewport->height = static_cast<float>(swapchainInfo.imageExtent.height);
    viewport->minDepth = 0.0f;
    viewport->maxDepth = 1.0f;

    scissor->offset = {0, 0};
    scissor->extent = swapchainInfo.imageExtent;

    VkPipelineViewportStateCreateInfo ans = {};
    ans.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    ans.pNext = 0;
    ans.flags = 0;
    ans.viewportCount = 1;
    ans.pViewports = viewport;
    ans.scissorCount = 1;
    ans.pScissors = scissor;
    return ans;
}

/**
 * @param colorBlendAttachmentState passed in and used the return value as pAttachment; due to lifetime constraints.
 */
static VkPipelineColorBlendStateCreateInfo createColorBlendStateCreateInfo(
    bool enableAlhpaBlending,
    VkPipelineColorBlendAttachmentState *colorBlendAttachmentState) {

    colorBlendAttachmentState->colorWriteMask = VK_COLOR_COMPONENT_R_BIT 
                                        | VK_COLOR_COMPONENT_G_BIT 
                                        | VK_COLOR_COMPONENT_B_BIT 
                                        | VK_COLOR_COMPONENT_A_BIT;

    if (!enableAlhpaBlending) {
        colorBlendAttachmentState->blendEnable = VK_FALSE;
    } else {
        colorBlendAttachmentState->blendEnable = VK_TRUE;
        colorBlendAttachmentState->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachmentState->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachmentState->colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachmentState->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachmentState->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachmentState->alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {};
    colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
    colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
    colorBlendStateCreateInfo.attachmentCount = 1;
    colorBlendStateCreateInfo.pAttachments = colorBlendAttachmentState;
    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

    return colorBlendStateCreateInfo;
}

PipelineInfo createPipeline(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo
) {
    PipelineInfo ans = {};
    ans.sampleCount = VK_SAMPLE_COUNT_1_BIT;

    // this will be parameterizd once
    ans.shaderModules = {
        .vertex = createShaderModuleFromSpirvFile(
            device, "shaders/vert.spv"),
        .fragment = createShaderModuleFromSpirvFile(
            device, "shaders/frag.spv")
    };

    vector<VkPipelineShaderStageCreateInfo> shaderStages = createShaderStageCreateInfos(
        ans.shaderModules);
    
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

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = createViewportStateCreateInfo(
        swapchainInfo,
        &ans.viewport,
        &ans.scissor);

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
    rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationStateCreateInfo.depthClampEnable = VK_FALSE; // discard instead of clamp
    rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationStateCreateInfo.lineWidth = 1.0f;
    rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
    rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {};
    multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateCreateInfo.rasterizationSamples = ans.sampleCount;
    multisampleStateCreateInfo.minSampleShading = 1.0f;
    multisampleStateCreateInfo.pSampleMask = VK_NULL_HANDLE;
    multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = createColorBlendStateCreateInfo(
        true, &ans.colorBlendAttachmentState);

    ans.layout = createPipelineLayout(device);
    ans.renderPass = createRenderPass(device, swapchainInfo, ans.sampleCount);

    // VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT
    //                                  , VK_DYNAMIC_STATE_SCISSOR
    //                                  , VK_DYNAMIC_STATE_LINE_WIDTH };

    // VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    // dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicStateCreateInfo.dynamicStateCount = 3;
    // dynamicStateCreateInfo.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
    pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
    pipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
    pipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
    pipelineCreateInfo.pDynamicState = VK_NULL_HANDLE;
    pipelineCreateInfo.layout = ans.layout;
    pipelineCreateInfo.renderPass = ans.renderPass;
    pipelineCreateInfo.subpass = 0;
    // pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
    pipelineCreateInfo.pDynamicState = VK_NULL_HANDLE;

    VkResult result = vkCreateGraphicsPipelines(
        device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &ans.pipeline);

    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create pipeline: " + toMessage(result));

    return ans;
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
        "failed to create shader module from spirv file - " + filePath + toMessage(result));

    return ans;
}

VkPipelineLayout createPipelineLayout(
    const VkDevice &device
) {
    // TODO: once we will pass stuff to the pipeline we should change this 
    VkPipelineLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.setLayoutCount = 0;
    layoutCreateInfo.pSetLayouts = NULL;
    layoutCreateInfo.pushConstantRangeCount = 0;
    layoutCreateInfo.pPushConstantRanges = NULL;

    VkPipelineLayout ans;
    VkResult result = vkCreatePipelineLayout(device, &layoutCreateInfo, NULL, &ans);

    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create pipeline layout: " + toMessage(result));

    return ans;
}

VkRenderPass createRenderPass(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo,
    VkSampleCountFlagBits sampleCount
) {
    VkAttachmentDescription attachmentDescriptions[1] = {};

    // color attachment description
    attachmentDescriptions[0].format = swapchainInfo.surfaceFormat.format;
    attachmentDescriptions[0].samples = sampleCount;
    attachmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pInputAttachments = VK_NULL_HANDLE; // TODO: once upon a time
    subpassDescription.pResolveAttachments = VK_NULL_HANDLE; // TODO: once upon a time
    subpassDescription.pDepthStencilAttachment = VK_NULL_HANDLE; // TODO: once upon a time
    subpassDescription.pPreserveAttachments = VK_NULL_HANDLE; // TODO: once upon a time

    // a new swapchain image acquisition is synchronized with the color attachment output stage;
    // we need the render pass to wait for this stage so it'll have a valid image to render to
    // and handle the transition. 
    VkSubpassDependency subpassDependency = {};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL; // implicit subpass, prior to pipeline
    subpassDependency.dstSubpass = 0; // our subpass, defined above
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = attachmentDescriptions;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;


    VkRenderPass ans;
    VkResult result = vkCreateRenderPass(device, &renderPassCreateInfo, NULL, &ans);

    if (result != VK_SUCCESS) throw std::runtime_error(
        "failed to create render pass: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
