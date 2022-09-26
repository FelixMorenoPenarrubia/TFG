#!/bin/bash
for FILE in canvases/*; do
    echo $FILE;
    swipl -s canvas_criticality_verifier -g "main('$FILE'),halt.";
done
