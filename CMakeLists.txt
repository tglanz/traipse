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

# global paths
link_libraries(vulkan)
include_directories(PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/third_party/vulkan/1.2.148.1/x86_64/include
    ${PROJECT_SOURCE_DIR}/third_party/glfw/3.3.2/include)

# include glfw in the project
# -- don't build any of glfw extras except for glfw itself
# -- dynamially link against vulkan
# -- libglfw.so
set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/glfw/3.3.2")

# after all dependencies, be strict regarding code quality
# if we have put it above the dependencies we will get errors regarding them, not cool
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# -- libtraipse_core.so
file(GLOB_RECURSE core_source_files "${PROJECT_SOURCE_DIR}/src/traipse/core/*.cpp")
message("core files: ${core_source_files}")
add_library(traipse_core SHARED ${core_source_files})
target_link_libraries(traipse_core glfw)

# -- traipse_basic
add_executable(traipse_basic ${PROJECT_SOURCE_DIR}/src/traipse/executables/basic.cpp) 
target_link_libraries(traipse_basic traipse_core)
