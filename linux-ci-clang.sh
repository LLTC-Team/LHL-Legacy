#!/usr/bin/env bash

mkdir clang
cd clang

svn co http://llvm.org/svn/llvm-project/llvm/tags/RELEASE_500/final llvm

cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/tags/RELEASE_500/final clang
cd ../..

cd llvm/tools/clang/tools
svn co http://llvm.org/svn/llvm-project/clang-tools-extra/tags/RELEASE_500/final extra
cd ../../../..

cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/tags/RELEASE_500/final compiler-rt
svn co http://llvm.org/svn/llvm-project/libcxx/tags/RELEASE_500/final libcxx
svn co http://llvm.org/svn/llvm-project/libcxxabi/tags/RELEASE_500/final libcxxabi
cd ../..

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release ../llvm
make && make install