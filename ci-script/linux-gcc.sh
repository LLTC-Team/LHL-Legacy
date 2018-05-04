#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y gcc-7 g++-7

sudo echo "export CC=gcc-7" >> /etc/profile
sudo echo "export CXX=g++-7" >> /etc/profile

source /etc/profile

gcc-7 -v
g++-7 -v

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh
