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

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <cassert>

// define __declspec for windows
#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

namespace LVM
{
	using LVMSDKFunction = std::function<void( std::vector<std::pair<void *, uint64_t>> )>;

	class Library
	{
	public:
		Library( const std::string &name, const std::string &author_name, const std::string &version,
				 const std::string &lvm_version, const std::map<std::string, LVMSDKFunction> &functions );

		const LVMSDKFunction &GetFunction( const std::string &name ) const;

	public:
		std::string m_Name;
		std::string m_AuthorName;
		std::string m_Version;
		std::string m_LVMVersion;
	private:
		std::map<std::string, LVMSDKFunction> m_Function;
	};

	LVM::Library::Library( const std::string &name, const std::string &author_name, const std::string &version,
						   const std::string &lvm_version, const std::map<std::string, LVMSDKFunction> &functions )
			: m_Name( name ), m_AuthorName( author_name ), m_Version( version ), m_LVMVersion( lvm_version ),
			  m_Function( functions )
	{

	}

	inline const LVMSDKFunction &Library::GetFunction( const std::string &name ) const
	{
		auto iter = m_Function.find( name );
		if (iter == m_Function.end())
		{
			std::cout << "can not find this function" << std::endl;
			abort();
		}
		return iter->second;
	}

#define LVM_LIBRARY_DECLARE const LVM::Library& GetLibrary();
#define LVM_LIBRARY_IMPLEMENT( lib_name, author_name, version, lvm_version, ... ) \
    const LVM::Library& GetLibrary() \
    { \
        static LVM::Library g_Library(lib_name,author_name,version,lvm_version,##__VA_ARGS__);\
        return g_Library;\
    }
#define LVM_LIBRARY_FUNCTION( func ) std::make_pair(#func,LVM::GetLibraryFunction(func))

	/*
	only use for function wrap
	*/
	template<typename T>
	inline T GetLibraryFunctionArgument( uint64_t cot, std::vector<std::pair<void *, uint64_t>> args )
	{
		using decay_type = typename std::decay<T>::type;
		if (sizeof( T ) == args[cot].second)
		{
			return ( *(decay_type *) ( args[cot].first ));
		} else
		{
			abort();
		}
	}

	/*
	only use for function wrap
	*/
	template<typename ReturnType, typename... ArgType>
	inline LVMSDKFunction GetLibraryFunction( ReturnType(*func)( ArgType... ))
	{
		return [=]( std::vector<std::pair<void *, uint64_t>> args )
		{
			assert( sizeof...( ArgType ) == args.size());
			uint64_t cot = 0;
			func( GetLibraryFunctionArgument<ArgType>( cot++, args )... );
		};
	}
}
