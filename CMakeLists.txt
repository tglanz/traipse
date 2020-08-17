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

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# file(GLOB_RECURSE core_source_files "${TRAIPSE_DIR}/core/*.cpp")
# add_library(traipse_core SHARED ${core_source_files})
# target_include_directories(traipse_core PUBLIC
#     ${PROJECT_SOURCE_DIR}/src/
#     ${PROJECT_SOURCE_DIR}/third_party/vulkan/1.2.148.1/x86_64/include)
# target_link_libraries(traipse vulkan)

add_compile_definitions(VK_USE_PLATFORM_XLIB_KHR)

file(GLOB_RECURSE core_source_files "${PROJECT_SOURCE_DIR}/src/traipse/core/*.cpp")
message("core files: ${core_source_files}")
add_library(traipse_core SHARED ${core_source_files})

link_libraries(vulkan)
include_directories(PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/third_party/vulkan/1.2.148.1/x86_64/include)


add_executable(traipse_basic ${PROJECT_SOURCE_DIR}/src/traipse/executables/basic.cpp) 
target_link_libraries(traipse_basic traipse_core)


# target_link_libraries(traipse_basic vulkan)
