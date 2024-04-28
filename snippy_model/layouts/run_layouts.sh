#!/bin/bash

for file in ./*.yaml; do
    echo -e "\e[34m$file\e[0m"
    llvm-snippy $file -o ./bin/
    echo
done