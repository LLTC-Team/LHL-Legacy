#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y gcc-7 g++-7

# setup doxygen
sudo apt-get install -y graphviz doxygen

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh