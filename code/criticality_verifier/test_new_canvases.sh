#!/bin/bash
for FILE in new_canvases/*; do
    echo $FILE;
    swipl -s canvas_criticality_verifier -g "main('$FILE'),halt.";
done
