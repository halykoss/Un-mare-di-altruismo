#!/bin/bash       
myArray=( "$@" )
DIR="bin"
builder="-build"
removecode="-clean"
EXECUTABLE="a.out"

if [[ " ${myArray[@]} " =~ " ${removecode} " ]]; then
    rm -rf $DIR $EXECUTABLE
    exit 0
fi

if ! [ -d "$DIR" ]; then
    mkdir $DIR
fi

cd $DIR
cmake ..

if [[ " ${myArray[@]} " =~ " ${builder} " ]]; then
    make
    mv $EXECUTABLE ..
fi


cd ..
