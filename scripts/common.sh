#!/bin/bash

func_info() {
    echo -e "\e[32m[INFO] $1"
}

func_error() {
    echo -e "\e[31m[EROR] $1"
    exit 1
}
