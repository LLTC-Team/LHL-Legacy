#!/usr/bin/env bash

apt-get remove cmake
apt-get purge --auto-remove cmake

curl -o cmake-install.sh https://cmake.org/files/v3.11/cmake-3.11.1-Linux-x86_64.sh
chmod +x ./cmake-install.sh
mkdir /opt/cmake
./cmake-install.sh --prefix /opt/cmake --skip-license
ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake

cmake -version