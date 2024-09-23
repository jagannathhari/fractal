#!/bin/bash

set -xe

INCLUDE_PATH_RAYLIB="./raylib-5.0_linux_amd64/include/"
INCLUDE_PATH="./src/include/"
LIB_PATH="./raylib-5.0_linux_amd64/lib/"


mkdir -p ./bin


gcc "./src/fractal.c" "./src/world.c" -o ./bin/fractal -I$INCLUDE_PATH_RAYLIB -I$INCLUDE_PATH -L$LIB_PATH -l:libraylib.a -lm
