#!/usr/bin/env bash

apt-get update
apt-get upgrade
apt-get install xz-utils build-essential

curl -o clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz http://releases.llvm.org/6.0.0/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
tar xf clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
mv clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04 /usr/local/clang_6.0.0

export PATH=/usr/local/clang_6.0.0/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/clang_6.0.0/lib:$LD_LIBRARY_PATH

clang -v
clang++ -v

export CC=clang
export CXX=clang++

chmod +x ./install-cmake.sh
./install-cmake.sh