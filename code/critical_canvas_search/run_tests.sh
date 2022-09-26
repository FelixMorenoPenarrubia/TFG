#!/bin/bash
make debug
for FILE in tests/*.inp; do
    TEST=$(basename $FILE .inp)
    ./debug < ./tests/$TEST.inp > temp.txt 2> /dev/null
    diff temp.txt ./tests/$TEST.cor
    if [ $? -ne 0 ]; then
        echo "INCORRECT TEST: $TEST"
    fi
done