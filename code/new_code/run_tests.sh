#!/bin/bash
make debug_sanitizer
for FILE in tests/*.inp; do
    TEST=$(basename $FILE .inp)
    ./debug < ./tests/$TEST.inp > ./f/temp.txt 2> /dev/null
    diff ./f/temp.txt ./tests/$TEST.cor
    if [ $? -ne 0 ]; then
        echo "INCORRECT TEST: $TEST"
    fi
done