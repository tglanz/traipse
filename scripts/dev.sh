#!/bin/bash

script_dir="$(realpath $(dirname $0))"
root_dir="$(realpath $script_dir/..)"
source_dir="$(realpath $root_dir/src)"

. $script_dir/common.sh

flag_lint=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -l|--lint) flag_lint=true; shift;;
        *) func_error "unknown argument: $1";;
    esac
done

func_lint() {
    cpplint --root="$source_dir" $(find $source_dir/** -name "*.*") 2>&1
}

func_execute_once() {
    clear
    [[ $flag_lint == true ]] && func_lint || return; 
    # add here more steps
}

func_execute_once
