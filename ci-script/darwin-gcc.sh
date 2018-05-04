#!/usr/bin/env bash

brew update
rm '/usr/local/include/c++'
brew install gcc

sudo mv /usr/bin/gcc /usr/bin/gcc-old
sudo mv /usr/bin/g++ /usr/bin/g++-old

sudo ln -s /usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7 /usr/bin/gcc
sudo ln -s /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7 /usr/bin/g++

gcc -v
g++ -v