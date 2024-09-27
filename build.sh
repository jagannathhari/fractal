#!/bin/bash

set -xe

ARCH=$(uname -m)
OS=$(uname)

RAYLIB_VERSION="5.0"

if [[ "$OS" == "Linux" ]]; then
    if [[ "$ARCH" == "x86_64" ]]; then
        RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_linux_amd64.tar.gz"
        RAYLIB_FOLDER="raylib-${RAYLIB_VERSION}_linux_amd64"
    elif [[ "$ARCH" == "i386" || "$ARCH" == "i686" ]]; then
        RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_linux_i386.tar.gz"
        RAYLIB_FOLDER="raylib-${RAYLIB_VERSION}_linux_i386"
    else
        echo "Unsupported architecture: $ARCH"
        exit 1
    fi
elif [[ "$OS" == "Darwin" ]]; then
    RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_macos.tar.gz"
    RAYLIB_FOLDER="raylib-${RAYLIB_VERSION}_macos"
elif [[ "$OS" == *"NT"* ]]; then
    if [[ "$ARCH" == "x86_64" ]]; then
        RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_windows_amd64.zip"
        RAYLIB_FOLDER="raylib-${RAYLIB_VERSION}_windows_amd64"
    elif [[ "$ARCH" == "i386" ]]; then
        RAYLIB_URL="https://github.com/raysan5/raylib/releases/download/${RAYLIB_VERSION}/raylib-${RAYLIB_VERSION}_windows_i386.zip"
        RAYLIB_FOLDER="raylib-${RAYLIB_VERSION}_windows_i386"
    else
        echo "Unsupported architecture: $ARCH"
        exit 1
    fi
else
    echo "Unsupported OS: $OS"
    exit 1
fi

if [ ! -d "$RAYLIB_FOLDER" ]; then
    echo "Downloading raylib..."
    if command -v wget > /dev/null; then
        wget $RAYLIB_URL -O raylib_archive
    else
        curl -L $RAYLIB_URL -o raylib_archive
    fi

    if [ $? -ne 0 ]; then
        echo "Failed to download raylib."
        exit 1
    fi

    if [[ $RAYLIB_URL == *.tar.gz ]]; then
        tar -xzf raylib_archive
    elif [[ $RAYLIB_URL == *.zip ]]; then
        unzip raylib_archive
    fi
    rm raylib_archive
fi

INCLUDE_PATH_RAYLIB="./$RAYLIB_FOLDER/include/"
INCLUDE_PATH="./src/include/"
LIB_PATH="./$RAYLIB_FOLDER/lib/"

mkdir -p ./bin

SOURCES="./src/fractal.c ./src/world.c"

if [[ "$OS" == "Linux" || "$OS" == "Darwin" ]]; then
    cc $SOURCES -o ./bin/fractal -I$INCLUDE_PATH_RAYLIB -I$INCLUDE_PATH -L$LIB_PATH -l:libraylib.a -lm
elif [[ "$OS" == *"NT"* ]]; then
    gcc $SOURCES -o ./bin/fractal.exe -I$INCLUDE_PATH_RAYLIB -I$INCLUDE_PATH -L$LIB_PATH -l:libraylib.a -lm -lopengl32 -lgdi32 -lwinmm
fi
