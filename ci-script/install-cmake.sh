#!/usr/bin/env bash

apt-get remove cmake
apt-get purge --auto-remove cmake

mkdir $HOME/cmake
export PATH="$HOME/cmake/bin:$PATH"
wget https://cmake.org/files/v3.12/cmake-3.12.3-Linux-x86_64.sh
chmod +x cmake-3.12.3-Linux-x86_64.sh
./cmake-3.12.3-Linux-x86_64.sh --prefix=$HOME/cmake --exclude-subdir --skip-license

cmake -version