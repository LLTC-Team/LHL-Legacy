#!/usr/bin/env bash

brew update
rm '/usr/local/include/c++'
brew install gcc
export CC=gcc-7
export CXX=g++-7