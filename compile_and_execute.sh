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
    meson "$1/build"
    build $1 $2
fi