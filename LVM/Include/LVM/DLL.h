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
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace LVM
{
#ifdef _WIN32
	class DLL
	{
	public:
		DLL(const std::string& filename);
		~DLL();
		void* GetAddress(const std::string& name);
	private:
		HMODULE m_Content;
	};
#else
	class DLL
	{
	public:
		DLL(const std::string& filename);
		~DLL();
		void* GetAddress(const std::string& name);
	private:
		void* m_pContent;
	};
#endif
}