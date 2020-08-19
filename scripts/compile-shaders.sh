#!/bin/bash

script_dir="$(realpath $(dirname $0))"
root_dir="$(realpath $script_dir/..)"
shaders_dir="$(realpath $root_dir/shaders)"

. $script_dir/common.sh

glslc --version >/dev/null 2>&1 || func_error "glslc cannot be found, try adding to your path, for example: PATH=/home/tal/vulkan/1.2.148.1/x86_64/bin:$PATH ./scripts/compile-shaders.sh"

for name in "vert" "frag"; do
    shader=$shaders_dir/shader.$name
    spirv=$shaders_dir/$name.spv

    func_info "compiling $shader to $spirv"
    glslc $shader -o $spirv || func_error "failed to compile: $shader"
done
