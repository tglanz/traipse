#!/bin/bash

func_verbose() {
    [[ $verbose_enabled == true ]] && echo -e "\e[34m[VERB] $1\e[0m"
}

func_info() {
    echo -e "\e[32m[INFO] $1\e[0m"
}

func_error() {
    echo -e "\e[31m[EROR] $1\e[0m"
    exit 1
}
