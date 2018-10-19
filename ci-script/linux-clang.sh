#!/usr/bin/env bash

sudo apt-get update
sudo apt-get install xz-utils build-essential

wget http://releases.llvm.org/6.0.0/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
tar xf clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
sudo mv clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04/ /usr/local/clang_6.0.0

chmod +x ./install-cmake.sh
./install-cmake.sh