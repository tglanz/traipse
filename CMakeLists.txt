cmake_minimum_required(VERSION 3.10)

project(
    traipse  
    VERSION 0.0.1
    DESCRIPTION "description for traipse"
    LANGUAGES CXX)

# uncomment to display all variables
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# THIS will make binaries to be saved at /bin
# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# THIS will make binaries to be saved to /build/bin (if you built at build)
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE source_files
    "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(traipse ${source_files})

target_link_libraries(
    traipse
    vulkan)

target_include_directories(
    traipse
    PUBLIC
    ${PROJECT_SOURCE_DIR}/third_party/vulkan/1.2.148.1/x86_64/include)
