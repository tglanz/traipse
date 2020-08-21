#include "traipse/application/application.h"
#include "traipse/application/arguments.h"

#include <spdlog/spdlog.h>
#include <string>

using namespace traipse::application;

int main(int argc, char **argv) {

    bool isError = false;
    Application application(2);
    Arguments arguments = parseArguments(argc, argv);

    spdlog::set_level(arguments.verbose
        ? spdlog::level::debug
        : spdlog::level::info);

    
    try {
        application.initWindow();
        application.initVulkan();
        application.mainLoop();
    } catch (const std::exception &exception) {
        spdlog::error(exception.what());
        isError = true;
    }

    application.cleanup();

    return isError ? 1 : 0;
}