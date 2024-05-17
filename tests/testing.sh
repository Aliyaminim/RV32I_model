#!/bin/bash

mkdir -p ../build/tests/output

echo -e "\e[1mTesting has started!\e[0m"
echo

for file in ./data/bin_files/*.bin; do
    echo $(basename "${file:17}")
    ../build/rv32i_sim $file ../build/tests/output/${file:17:-3}out
    diff answers/${file:17:-3}out ../build/tests/output/${file:17:-3}out
    if [ $? = 0 ]
    then
        echo "Correct answer"
    else
        echo "Incorrect answer"
    fi
    echo
done