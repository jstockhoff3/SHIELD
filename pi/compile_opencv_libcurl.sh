#!/bin/bash
echo "compiling $1"
if [[ $1 == *.c ]]
then
    gcc -ggdb `pkg-config --cflags opencv` -o `basename $1 .c` $1 `pkg-config --libs opencv libcurl jansson` -lwiringPi;
elif [[ $1 == *.cpp ]]
then
    g++ -ggdb `pkg-config --cflags opencv` -o `basename $1 .cpp` $1 `pkg-config --libs opencv libcurl jansson` -lwiringPi;
else
    echo "Please compile only .c or .cpp files"
fi
echo "Output file => ${1%.*}"