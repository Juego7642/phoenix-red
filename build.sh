#!/bin/bash

echo "Compiling game..."

gcc main.c -o main.exe -lraylib -lopengl32 -lgdi32 -lwinmm

if [ $? -eq 0 ]; then
    echo "Build successful!"
    ./main.exe
else
    echo "Build failed!"
fi