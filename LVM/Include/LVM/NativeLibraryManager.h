/*
Copyright 2018 creatorlxd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once

#include "stdafx.h"

// include related dynamic loading header file
#ifdef _WIN32
#include <Windows.h>
#else

#include <dlfcn.h>

#endif

// define dynamic library's type in c/cpp
#ifdef _WIN32
#define LIB_TYPE HMODULE
#else
#define LIB_TYPE void*
#endif

// define dynamic library's file extension
#ifdef _WIN32
#define DL_SUFFIX ".dll"
#endif
#ifdef __APPLE__
#define DL_SUFFIX ".dylib"
#endif
#ifdef __linux__
#define DL_SUFFIX ".so"
#endif
#ifndef DL_SUFFIX
#define DL_SUFFIX
#endif

// define dynamic library's prefix for *nix os 
#ifdef _WIN32
#define DL_PREFIX ""
#else
#define DL_PREFIX "lib"
#endif

namespace LVM
{
    class NativeLibraryManager
    {
    public:
        explicit NativeLibraryManager(const std::string &folder);

        ~NativeLibraryManager();

        void *GetNativeFunction(const std::string &library_name, const std::string &function_name);

    private:
        LIB_TYPE LoadNativeLibrary( const std::string &name );

        std::string m_Folder;
        std::map<std::string, LIB_TYPE> m_Libraries;
    };
}