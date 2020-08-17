#include "traipse/core/preferences.h"

using std::vector;

namespace traipse {
namespace core {

VkSurfaceFormatKHR chooseSurfaceFormat(vector<VkSurfaceFormatKHR> surfaceFormats) {
    // TODO: priority queue
    for (const auto& surfaceFormat : surfaceFormats) {
        if (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB) {
                return surfaceFormat;
            }
        }
    }

    return surfaceFormats.at(0);
}

VkPresentModeKHR choosePresentMode(vector<VkPresentModeKHR> presentModes) {
    // TODO: priority queue
    for (const auto& presentMode : presentModes) {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

}  // core
}  // traipse
