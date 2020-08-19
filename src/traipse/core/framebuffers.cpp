#include "traipse/core/framebuffers.h"
#include "traipse/core/slut.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <stdexcept>

using std::vector, std::string;

namespace traipse {
namespace core {

vector<VkFramebuffer> createFramebuffers(
    const VkDevice &device,
    const SwapchainInfo &swapchainInfo,
    const PipelineInfo &pipelineInfo
){
    vector<VkFramebuffer> ans(swapchainInfo.imageViews.size());

    for (size_t idx = 0; idx < swapchainInfo.imageViews.size(); ++idx) {
        vector<VkImageView> attachments = {
            swapchainInfo.imageViews.at(idx)
        };

        VkFramebufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = pipelineInfo.renderPass;
        createInfo.width = swapchainInfo.imageExtent.width;
        createInfo.height = swapchainInfo.imageExtent.height;
        createInfo.attachmentCount = attachments.size();
        createInfo.pAttachments = attachments.data();
        createInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(device, &createInfo, NULL, &ans.at(idx));

        if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create framebuffer: " + toMessage(result));
    }

    return ans;
}

}  // core
}  // traipse
