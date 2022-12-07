#!/bin/bash
for FILE in new_triangle_graphs/*; do
    echo $FILE;
    swipl --stack_limit=8G -s arbitrary_criticality_verifier -g "main('$FILE'),halt.";
done
