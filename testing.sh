#!/bin/bash
echo
echo "Testing ADD:"
for file in tests/001_add.in; do
    echo $file
    ./build/main < $file;
    echo
done

echo "Testing SUB:"
for file in tests/002_sub.in; do
    echo $file
    ./build/main < $file;
    echo
done

echo "Testing ADDI:"
for file in tests/003_addi.in; do
    echo $file
    ./build/main < $file;
    echo
done