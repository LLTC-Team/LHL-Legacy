#!/usr/bin/env bash

brew update
rm '/usr/local/include/c++'
brew install gcc

sudo echo "export CC=gcc-7" >> ~/.profile
sudo echo "export CXX=g++-7" >> ~/.profile

source ~/.profile