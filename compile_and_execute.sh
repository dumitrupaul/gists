#!/bin/bash

function build {
    if ninja -C build ; then
        cd build
        ./demo
    fi
}

if [[ -d build ]]
then
    build
else
    meson build
    build
fi