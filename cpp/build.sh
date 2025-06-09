#!/bin/sh

rm -rf build/release

mkdir -p build/release

if [ ! -d build/examples ]; then
    ln -s ../../examples build/examples
fi

cd build/release

cmake ../..

make

cp lib/libminion_gui.so ../../../go/mugui/lib/libminion_gui.so
