#!/bin/bash
for FILE in wedges/*; do
    echo $FILE;
    swipl --stack_limit=8G -s arbitrary_criticality_verifier_listsizes -g "main('$FILE'),halt.";
done
