#!/usr/bin/env bash

apt-get remove cmake
apt-get purge --auto-remove cmake

wget https://cmake.org/files/v3.11/cmake-3.11.1-Linux-x86_64.sh
chmod +x ./cmake-3.11.1-Linux-x86_64.sh
mkdir /opt/cmake
./cmake-3.11.1-Linux-x86_64.sh --prefix /opt/cmake --skip-license
ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

cmake -version