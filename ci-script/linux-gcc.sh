#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y gcc-7 g++-7

sudo echo "export CC=gcc-7" >> ~/.profile
sudo echo "export CXX=g++-7" >> ~/.profile

source ~/.profile

chmod +x ./install-cmake.sh
sudo ./install-cmake.sh
