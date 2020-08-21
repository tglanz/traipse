#include "traipse/application/arguments.h"
#include <string>

using std::string;

namespace traipse {
namespace application {

Arguments parseArguments(int argc, const char * const *argv) {
    Arguments arguments = {};

    for (int index = 1; index < argc; ++index) {
        string argument = argv[index];

        if (argument == "-v" || argument == "--verbose") {
            arguments.verbose = true;
        }
    }

    return arguments;
}

}  // namespace application
}  // namespace traipse