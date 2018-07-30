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
#include "Command.h"

namespace LVM
{
	typedef uint64_t AddressType;

	class MemoryManager;

	struct MemoryAddressArgument;

	class MemoryPage
	{
	public:
		friend class MemoryManager;

		static const AddressType MemoryPageDefaultSize = 0XFFFFF;

		MemoryPage();

		~MemoryPage();

		void Allocate( AddressType size = MemoryPageDefaultSize );

		void Release();

		AddressType GetSize();

		template<typename T>
		T &GetContent( AddressType address )
		{
			if (m_pContent)
			{
				return *( reinterpret_cast<T *>(m_pContent + address));
			} else
			{
				ThrowError( "can not get content from a null memory page" );
				return T();
			}
		}

	private:
		Byte *m_pContent;
		AddressType m_Size;
	};

	using MemoryLink = std::function<void *()>;

	class MemoryManager
	{
	public:
		MemoryManager( AddressType size = MemoryPage::MemoryPageDefaultSize );

		Byte *GetContent( const std::vector<MemoryAddressArgument> &maa, SizeType size = 1 );

		template<typename T>
		T &GetContent( const std::vector<MemoryAddressArgument> &maa )
		{
			return *( reinterpret_cast<T *>(GetContent( maa, sizeof( T ))));
		}

		void AddLink( AddressType address, const MemoryLink &memory_link );

		AddressType GetPageSize();

	private:
		Byte *GetMemoryContent( AddressType address, SizeType size = 1 );

		template<typename T>
		T &GetMemoryContent( AddressType address )
		{
			return *( reinterpret_cast<T *>(GetMemoryContent( address, sizeof( T ))));
		}

		Byte *GetLinkContent( AddressType address );

		template<typename T>
		T &GetLinkContent( AddressType address )
		{
			return *( reinterpret_cast<T *>(GetLinkContent( address )));
		}

	private:
		AddressType m_PageSize;
		std::vector<MemoryPage> m_Page;
		std::map<AddressType, MemoryLink> m_Link;
	};

	enum class MemoryAddressArgumentType : Byte
	{
		Default = 0,
		Jump = 1,
		Memory = 2,
		Link = 3
	};

	struct MemoryAddressArgument
	{
		MemoryAddressArgument() = default;

		MemoryAddressArgument( AddressType address );

		MemoryAddressArgument( AddressType address, MemoryAddressArgumentType type );

		AddressType m_Content = 0;
		MemoryAddressArgumentType m_Type = MemoryAddressArgumentType::Default;
	};

	bool operator==( const MemoryAddressArgument &maa1, const MemoryAddressArgument &maa2 );

	Argument MemoryAddressArgumentToArgument( const std::vector<MemoryAddressArgument> &maa );

	std::vector<MemoryAddressArgument> ArgumentToMemoryAddressArgument( const Argument &arg );
}
