#!/usr/bin/env bash

echo "Building CMake from source"

ehco "Downloading CMake source"
curl -o cmake-3.11.0.tar.gz https://cmake.org/files/v3.11/cmake-3.11.0.tar.gz
tar -zxf cmake-3.11.0.tar.gz

ehco "Building starts"
cd cmake-3.11.0
./bootstrap && make && make install

echo "CMake has been built successfully"