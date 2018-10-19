#!/usr/bin/env bash

sudo apt remove cmake
sudo apt purge --auto-remove cmake

mkdir $HOME/cmake
wget https://cmake.org/files/v3.12/cmake-3.12.3-Linux-x86_64.sh
chmod +x cmake-3.12.3-Linux-x86_64.sh
./cmake-3.12.3-Linux-x86_64.sh --prefix=$HOME/cmake --exclude-subdir --skip-license

cmake --version