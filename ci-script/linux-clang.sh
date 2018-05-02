#!/usr/bin/env bash

sudo apt-get update
sudo apt-get install xz-utils build-essential

curl -o clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz http://releases.llvm.org/6.0.0/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz
tar xf clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz
mv clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04/ /usr/local/clang_6.0.0

sudo echo "export PATH=/usr/local/clang_6.0.0/bin:\$PATH" >> ~/.profile
sudo echo "export LD_LIBRARY_PATH=/usr/local/clang_6.0.0/lib:\$LD_LIBRARY_PATH" >> ~/.profile
sudo echo "export CC=clang-6" >> ~/.profile
sudo echo "export CXX=clang-6" >> ~/.profile

source ~/.profile

clang -v
clang++ -v

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh