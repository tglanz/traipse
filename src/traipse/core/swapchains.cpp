#include "traipse/core/swapchains.h"
#include "traipse/core/surfaces.h"
#include "traipse/core/preferences.h"
#include "traipse/core/slut.h"

#include <stdexcept>

namespace traipse {
namespace core {

SwapchainInfo createSwapchain(
        const VkDevice &device,
        const VkPhysicalDevice &physicalDevice,
        const VkSurfaceKHR &surface) { 

    PhysicalDeviceSurfaceInfo physicalDeviceSurfaceInfo = getPhysicalDeviceSurfaceInfo(
           physicalDevice, surface); 

    SwapchainInfo ans = {
        .swapchain = VK_NULL_HANDLE,
        .surfaceFormat = chooseSurfaceFormat(physicalDeviceSurfaceInfo.surfaceFormats),
        .imageExtent = physicalDeviceSurfaceInfo.capabilities.currentExtent
    };

    VkResult result;

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.pNext = NULL;
    swapchainCreateInfo.surface = surface;

    // parameters for creation
    swapchainCreateInfo.flags = 0;

    // at least many images for the swapchain 
    swapchainCreateInfo.minImageCount = physicalDeviceSurfaceInfo.capabilities.minImageCount; 

    // format that the swapchain images will be created with 
    swapchainCreateInfo.imageFormat = ans.surfaceFormat.format; 
    swapchainCreateInfo.imageColorSpace = ans.surfaceFormat.colorSpace;

    // size of the swapchain images
    swapchainCreateInfo.imageExtent = ans.imageExtent;
    if (swapchainCreateInfo.imageExtent.width == 0xffffffff) throw std::runtime_error(
           "unhandled case of when the image extent is not initialized"); 

    // used for stereo surface; for us - always one
    swapchainCreateInfo.imageArrayLayers = 1;

    // how we gonna use the image
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // how different queue families can access the image
    // note that currently we use a single queue family, if changed this should change too
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // number of different queue families to concurrently access the images 
    swapchainCreateInfo.queueFamilyIndexCount = 0;

    // the indices of the queue families to concurrently access the images
    swapchainCreateInfo.pQueueFamilyIndices = NULL;

    // image transformation prior to presentation
    swapchainCreateInfo.preTransform = physicalDeviceSurfaceInfo.capabilities.currentTransform;

    // determines how compositing is made on the surface
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // determines how presentation requests are handled in the swapchain
    swapchainCreateInfo.presentMode = choosePresentMode(physicalDeviceSurfaceInfo.presentModes);

    // enable clipping of un-needed regions of the rendering
    swapchainCreateInfo.clipped = VK_TRUE;

    // no previous swapchains
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, NULL, &ans.swapchain); 
    if (result != VK_SUCCESS) throw std::runtime_error(
            "failed to create swapchain: " + toMessage(result));

    return ans;
}

}  // core
}  // traipse
