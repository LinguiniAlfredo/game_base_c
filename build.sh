#!/bin/bash


echo "Compiling..."
SECONDS=0
gcc main.c -o game -lSDL2 -lSDL2_image -lSDL2_ttf
printf "Finished in %.3f seconds" "$SECONDS"
