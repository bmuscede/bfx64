#!/bin/bash

#First, run cmake to build the program.
echo "Building Project..."
cmake .
make

echo ""

#Cleans the project.
echo "Cleaning project..."
#make clean
rm CMakeCache.txt Makefile cmake_install.cmake 
rm -r CMakeFiles/
