#!/usr/bin/env sh

# get the absolute path to the script's directory
dir_root="$(cd "$(dirname "$0")"; pwd -P)"

docker build --rm --tag=blue-pill:mini-blink "${dir_root}"


# vim: set ts=2 sw=2 et:


