#include <vulkan/vulkan.h>
#include <iostream>
#include <exception>
#include <string>
#include <variant>

#include "traipse/core/core.h"

using namespace std;
using namespace traipse::core;

void cleanup(const VkInstance &instance) {
    vkDestroyInstance(instance, NULL);
}

int main(int argc, char** argv) {
    const auto applicationInfo = createApplicationInfo();
    const auto instanceCreateInfo = createInstanceCreateInfo(&applicationInfo);

    VkResult result;
    VkInstance instance;

    try {
        result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
        cout << "create instance result: " << toMessage(result) << endl;

        if (result != VK_SUCCESS) {
            throw "failed to create instance";
        }

        cout << "acquiring layers properties" << endl;
        vector<LayerProperties> layersProperties;
        {
            auto response = acquireLayersProperties();
            if (auto ptr = std::get_if<vector<LayerProperties>>(&response)) {
                layersProperties = *ptr;
            } else {
                result = std::get<VkResult>(response);
                if (result != VK_SUCCESS) throw std::runtime_error(
                       "failed to acquire layers properties: " + toMessage(result)); 
            }
        }

        cout << "acquiring physical devices" << endl;
        vector<VkPhysicalDevice> physicalDevices;
        {
            auto response = acquirePhysicalDevices(instance);

            if (auto ptr = std::get_if<vector<VkPhysicalDevice>>(&response)) {
                physicalDevices = *ptr;
            } else {
                result = std::get<VkResult>(response);
                if (result == VK_SUCCESS) {
                    throw std::runtime_error("no physical devices were found");
                } else {
                    throw std::runtime_error("failed to acquire physical devices: " + toMessage(result));
                }
            }
            cout << "physical devices:" << endl;
            for (auto physicalDevice : physicalDevices) {
                cout << " - ..." << endl;
            }
        }

    } catch (const std::exception &ex) {
        cleanup(instance);
        cerr << "error: " << ex.what() << endl;
        exit(1);
    }


    return 0;
}
