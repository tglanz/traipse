#include "traipse/application/application.h"

#include <iostream>

using traipse::application::Application;

int main() {
    bool isError = false;
    Application application;
    
    try {
        application.initWindow();
        application.initVulkan();
        application.mainLoop();
    } catch (const std::exception &exception) {
        std::cerr << "error: " << exception.what() << std::endl;
        isError = true;
    }

    application.cleanup();

    return isError ? 1 : 0;
}