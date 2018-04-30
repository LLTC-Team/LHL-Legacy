#!/usr/bin/env bash

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y gcc-7 g++-7

CC=gcc-7
CXX=g++-7

chmod +x ./install-cmake.sh
./install-cmake.sh
