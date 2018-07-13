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
#include "LVM/stdafx.h"
#include "LVM/DLL.h"

LVM::DLL::DLL(const std::string & filename)
{

#ifdef _WIN32
	m_pContent = LoadLibraryA(filename.c_str());
#else
	m_pContent = dlopen(filename.c_str(), RTLD_NOW);
#endif

	if(!m_pContent)
		ThrowError("DLL " + filename + " Failed");
}

LVM::DLL::~DLL()
{
	if (m_pContent)
	{
#ifdef _WIN32
		FreeLibrary(m_pContent);
#else
		dlclose(m_pContent);
#endif
	}
}

void * LVM::DLL::GetAddress(const std::string & name)
{

#ifdef _WIN32
	void* re = GetProcAddress(m_pContent, name.c_str());
#else
	void* re = dlsym(m_pContent, name.c_str());
#endif

	if (re)
		return re;
	else
	{
		ThrowError("can not find " + name + " in dll");
		return nullptr;
	}
}