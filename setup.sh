#!/bin/bash       
myArray=( "$@" )
DIR="bin"
builder="-build"
removecode="-clean"
IMG="final_image.png"
EXECUTABLE="a.out"

# Se si vuole pulire l'albero
if [[ " ${myArray[@]} " =~ " ${removecode} " ]]; then
    rm -rf $DIR $EXECUTABLE  $IMG
    exit 0
fi
# Se si vuole fare il build del progetto
if ! [ -d "$DIR" ]; then
    mkdir $DIR
fi

cd $DIR
cmake ..

if [[ " ${myArray[@]} " =~ " ${builder} " ]]; then
    make
fi


cd ..
