#!/usr/bin/env bash

add-apt-repository ppa:ubuntu-toolchain-r/test
apt-get update
apt-get upgrade
apt-get install -y gcc-7 g++-7

export CC=gcc-7
export CXX=g++-7

chmod +x ./install-cmake.sh
./install-cmake.sh
