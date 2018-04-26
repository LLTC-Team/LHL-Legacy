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

	class MemoryPage
	{
	public:
		friend class MemoryManager;

		static const AddressType MemoryPageDefaultSize = 0XFFFFF;

		MemoryPage();
		~MemoryPage();
		void Allocate(AddressType size = MemoryPageDefaultSize);
		void Release();
		AddressType GetSize();

		template<typename T>
		T& GetContent(AddressType address)
		{
			if (m_pContent)
			{
				return *(reinterpret_cast<T*>(m_pContent + address));
			}
			else
			{
				ThrowError("can not get content from a null memory page");
				return T();
			}
		}
	private:
		Byte * m_pContent;
		AddressType m_Size;
	};

	class MemoryManager
	{
	public:
		MemoryManager(AddressType size = MemoryPage::MemoryPageDefaultSize);

		Byte* GetContent (AddressType address,size_t size = 1);

		template<typename T>
		T& GetContent(AddressType address)
		{
			return *(reinterpret_cast<T*>(GetContent(address,sizeof(T))));
		}
		AddressType GetPageSize();
	private:
		AddressType m_PageSize;
		std::vector<MemoryPage> m_Pages;
	};

	/*
	get argument from memory address vector
	*/
	Argument SetMemoryAddress(const std::vector<AddressType>& addrs, bool if_last_jump = false);

	/*
	Get Memory Address From Argument.
	Include Memory Address Jumping
	*/
	AddressType GetMemoryAddress(const Argument &arg, MemoryManager &memory_manager);
}
