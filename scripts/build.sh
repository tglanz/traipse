#!/bin/bash

script_dir="$(realpath $(dirname $0))"
root_dir="$(realpath $script_dir/..)"
source_dir="$(realpath $root_dir/src)"
build_dir="$(realpath $root_dir/build)"

. $script_dir/common.sh

flag_clean=false
flag_debug=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -c|--clean) flag_clean=true; shift;;
        -d|--debug) flag_debug=true; shift;;
        -v|--verbose) verbose_enabled=true; shift;;
        *) func_error "unknown argument: $1";;
    esac
done

[ $flag_debug == true ] && build_type=Debug || build_type=Release;

func_verbose "parameters:
  - source directory : $source_dir
  - build directory  : $build_dir
  - build type       : $build_type
  - clean enabled    : $flag_clean"
 
[[ $flag_clean == true ]] && rm -rf $build_dir
 
cmake -H$root_dir -B$build_dir -DCMAKE_BUILD_TYPE=$build_type && cmake --build $build_dir --config $build_type -- -j 4
