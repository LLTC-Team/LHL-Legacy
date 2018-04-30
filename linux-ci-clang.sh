#!/usr/bin/env bash

echo "Building LLVM and CLang from sources"

mkdir clang
cd clang

echo "Checkout LLVM"
svn export -q http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_500/final llvm

ehco "Checkout CLang"
cd llvm/tools
svn export -q http://llvm.org/svn/llvm-project/cfe/tags/RELEASE_500/final clang
cd ../..

ehco "Checkout CLang tools"
cd llvm/tools/clang/tools
svn export -q http://llvm.org/svn/llvm-project/clang-tools-extra/tags/RELEASE_500/final extra
cd ../../../..

ehco "Checkout compiler-rt and libc++"
cd llvm/projects
svn export -q http://llvm.org/svn/llvm-project/compiler-rt/tags/RELEASE_500/final compiler-rt
svn export -q http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_500/final libcxx
svn export -q http://llvm.org/svn/llvm-project/libcxxabi/tags/RELEASE_500/final libcxxabi
cd ../..

mkdir build
cd build

echo "Building begins"
cmake -DCMAKE_BUILD_TYPE=Release ../llvm
make && make install

echo "LLVM and CLang have been built successfully"