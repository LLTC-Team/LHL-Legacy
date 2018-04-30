#!/usr/bin/env bash

brew update
rm '/usr/local/include/c++'
brew install gcc
CC=gcc-7
CXX=g++-7