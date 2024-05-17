#!/bin/bash

for file in ./*.s; do
    riscv64-unknown-linux-gnu-as -march=rv32i -mabi=ilp32 $file -o ${file%.s}.o
    riscv64-unknown-linux-gnu-ld -march=rv32i -m elf32lriscv_ilp32  ${file%.s}.o -o ${file%.s}.elf
    riscv64-unknown-linux-gnu-objcopy -O binary ${file%.s}.elf ./bin_files/${file%.s}.bin
    rm *.o
    rm *.elf
done