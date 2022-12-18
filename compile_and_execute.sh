#!/bin/bash

function build {
    if ninja -C "$1/build" ; then
        cd "$1/build"
        printf '%*s\n' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
        ./$2
    fi
}

if [[ -d "$1/build" ]]
then
    build $1 $2
else
    pushd $1 > /dev/null
        meson build
    popd > /dev/null
    build $1 $2
fi