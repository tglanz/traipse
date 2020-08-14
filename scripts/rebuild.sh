#!/bin/bash

script_dir="$(realpath $(dirname $0))"
root_dir=$(realpath $script_dir/..)

rm -rf $root_dir/build

cmake -H$root_dir -B$root_dir/build && cmake --build $root_dir/build && echo "success" || "fail" 
