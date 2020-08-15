#include <vulkan/vulkan.h>
#include <iostream>
#include <string>
#include <variant>

#include "traipse/core/core.h"

using namespace std;
using namespace traipse::core;

int main(int argc, char** argv) {
    const auto applicationInfo = createApplicationInfo();
    const auto instanceCreateInfo = createInstanceCreateInfo(&applicationInfo);

    VkResult result;
    VkInstance instance;

    result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
    cout << "create instance result: " << toMessage(result) << endl;

    if (result != VK_SUCCESS) {
        return 1;
    }

    auto response = acquireLayersProperties();
    if (auto resultPtr = std::get_if<VkResult>(&response)) {
        cout << "failed to acquire layers properties: " << toMessage(*resultPtr) << endl;
    }


    vkDestroyInstance(instance, NULL);

    return 0;
}
