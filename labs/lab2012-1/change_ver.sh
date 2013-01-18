#!/bin/bash

lab_no=$1;
lab_part=$2;

cp "lab$lab_no-$(($lab_part-1)).c" "lab$lab_no-$lab_part.c"
cp "lab$lab_no-$(($lab_part-1)).vert" "lab$lab_no-$lab_part.vert"
cp "lab$lab_no-$(($lab_part-1)).frag" "lab$lab_no-$lab_part.frag"
