#!/bin/bash

echo "Compiling..."
gcc main.c -o game -lSDL2 -lSDL2_image -lSDL2_ttf
echo "Finished."
