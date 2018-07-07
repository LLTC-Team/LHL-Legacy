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

#ifdef _WIN32
LVM::DLL::DLL(const std::string & filename)
{
	m_Content = LoadLibraryA(filename.c_str());
	if (!m_Content)
		ThrowError("DLL " + filename + " Failed");
}

LVM::DLL::~DLL()
{
	if(m_Content)
		FreeLibrary(m_Content);
}

void * LVM::DLL::GetAddress(const std::string & name)
{
	void* re = GetProcAddress(m_Content, name.c_str());
	if (re)
		return re;
	else
	{
		ThrowError("can not find " + name + " in dll");
		return nullptr;
	}
}
#else
LVM::DLL::DLL(const std::string & filename)
{
	m_pContent = dlopen(filename.c_str(), RTLD_NOW);
	if(!m_pContent)
		ThrowError("DLL " + filename + " Failed");
}

LVM::DLL::~DLL()
{
	if (m_pContent)
		dlclose(m_pContent);
}

void * LVM::DLL::GetAddress(const std::string & name)
{
	void* re = dlsym(m_pContent, name.c_str());
	if (re)
		return re;
	else
	{
		ThrowError("can not find " + name + " in dll");
		return nullptr;
	}
}
#endif