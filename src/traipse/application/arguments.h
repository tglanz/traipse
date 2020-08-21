#ifndef TRAIPSE_APPLICATION_ARGUMENTS_H_
#define TRAIPSE_APPLICATION_ARGUMENTS_H_

namespace traipse {
namespace application {

struct Arguments {
    bool verbose;
};

Arguments parseArguments(int argc, const char * const *argv);
    
}  // namespace application
}  // namespace traipse

#endif  // #ifndef TRAIPSE_APPLICATION_ARGUMENTS_H_