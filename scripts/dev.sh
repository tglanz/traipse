#!/bin/bash

script_dir="$(realpath $(dirname $0))"
root_dir="$(realpath $script_dir/..)"
source_dir="$(realpath $root_dir/src)"

. $script_dir/common.sh

flag_lint=false
flag_generate=false
flag_build=fales

while [[ $# -gt 0 ]]; do
    case $1 in
        -l|--lint) flag_lint=true; shift;;
        -g|--generate) flag_generate=true; shift;;
        -b|--build) flag_build=true; shift;;
        *) func_error "unknown argument: $1";;
    esac
done

func_lint() {
    func_info "linting"
    cpplint --root="$source_dir" $(find $source_dir/** -name "*.*") 2>&1
}

func_generate() {
    func_info "generating"
    cmake -H$root_dir -B$root_dir/build
}

func_build() {
    func_info "building"
    cmake --build $root_dir/build
}

func_execute_once() {
    clear
    [[ $flag_lint == true ]] && func_lint || return; 
    [[ $flag_generate == true ]] && func_generate || return;
    [[ $flag_build == true ]] && func_build || return;
    
}

func_execute_once
