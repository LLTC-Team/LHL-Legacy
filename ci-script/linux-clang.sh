#!/usr/bin/env bash

sudo apt-get update
sudo apt-get install xz-utils build-essential

wget http://releases.llvm.org/6.0.0/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
tar xf clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
sudo mv clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04/ /usr/local/clang_6.0.0

sudo mv /usr/bin/gcc /usr/bin/gcc-old
sudo mv /usr/bin/g++ /usr/bin/g++-old

sudo ln -s /usr/local/clang_6.0.0/bin/clang /usr/bin/gcc
sudo ln -s /usr/local/clang_6.0.0/bin/clang++ /usr/bin/g++

gcc -v
g++ -v

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh