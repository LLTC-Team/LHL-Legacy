#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y gcc-7 g++-7

sudo mv /usr/bin/gcc /usr/bin/gcc-old
sudo mv /usr/bin/g++ /usr/bin/g++-old

sudo ln -s /usr/bin/gcc-7 /usr/bin/gcc
sudo ln -s /usr/bin/g++-7 /usr/bin/g++

gcc -v
g++ -v

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh