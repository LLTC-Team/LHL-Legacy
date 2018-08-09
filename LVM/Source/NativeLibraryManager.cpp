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
#include "LVM/NativeLibraryManager.h"

std::string getErrorMessage()
{
#ifdef _WIN32
	DWORD error = GetLastError();
	char buffer[100];
	int numbers = snprintf(buffer, sizeof(buffer), "Failed to load dynamic library with error code %lu", error);
	std::string error_msg(buffer, numbers);
	return error_msg;
#else
	std::string error_msg( dlerror());
	return error_msg;
#endif
}

LVM::NativeLibraryManager::NativeLibraryManager( const std::string &folder ) : m_Folder( folder ), m_Libraries()
{
	// check the last char for filename appending
	char endChar = *( m_Folder.end() - 1 );
	if (endChar == '\\')
	{
		*m_Folder.end() = '/';
	} else if (endChar != '/')
	{
		m_Folder.append( "/" );
	}
}

LVM::NativeLibraryManager::~NativeLibraryManager()
{
	for (auto &library : m_Libraries)
	{
#ifdef _WIN32
		FreeLibrary(library.second);
#else
		dlclose( library.second );
#endif
	}
}

LIB_TYPE LVM::NativeLibraryManager::LoadNativeLibrary( const std::string &name )
{
	std::string filename = m_Folder + DL_PREFIX + name + DL_SUFFIX;
	LIB_TYPE lib_ref = nullptr;
#ifdef _WIN32
	lib_ref = LoadLibraryA(filename.c_str());
#else
	lib_ref = dlopen( filename.c_str(), RTLD_NOW );
#endif
	if (lib_ref)
	{
		return lib_ref;
	} else
	{
		LVM::ThrowError( getErrorMessage());
		return nullptr; // To avoid warnings
	}
}

void *LVM::NativeLibraryManager::GetNativeFunction( const std::string &library_name, const std::string &function_name )
{
	LIB_TYPE library = nullptr;
	if (m_Libraries.count( library_name ) == 0)
	{
		m_Libraries[library_name] = ( library = LoadNativeLibrary( library_name ));
	} else
	{
		library = m_Libraries[library_name];
	}

#ifdef _WIN32
	void* func = GetProcAddress(library, function_name.c_str());
#else
	void *func = dlsym( library, function_name.c_str());
#endif

	if (func)
	{
		return func;
	} else
	{
		LVM::ThrowError( getErrorMessage());
		return nullptr;
	}
}

