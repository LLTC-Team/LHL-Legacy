﻿/*
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
#include "../Include/LVM/stdafx.h"
#include "../Include/LVM/Memory.h"

LVM::MemoryPage::MemoryPage()
{
	m_pContent = nullptr;
	m_Size = 0;
}

LVM::MemoryPage::~MemoryPage()
{
	if (m_pContent)
		delete[] m_pContent;
}

void LVM::MemoryPage::Allocate(AddressType size)
{
	if (m_pContent)
	{
		ThrowError("can not allocate for a non-empty memory page");
		return;
	}
	m_Size = size;
	m_pContent = new Byte[size];
	memset(m_pContent, 0, m_Size * sizeof(Byte));
}

void LVM::MemoryPage::Release()
{
	if (!m_pContent)
	{
		ThrowError("can not release a empty memory page");
		return;
	}
	delete[] m_pContent;
	m_pContent = nullptr;
}

LVM::AddressType LVM::MemoryPage::GetSize()
{
	return m_Size;
}

LVM::MemoryManager::MemoryManager(AddressType size)
{
	m_PageSize = size;
}

LVM::Byte* LVM::MemoryManager::GetContent(AddressType address,size_t size)
{
	uint32_t index = address / m_PageSize;
	if ((address + size - 1) / m_PageSize != index)		//size safe check
	{
		ThrowError("the size is larger that the memory left in the memory page");
		return nullptr;
	}
	if (m_Page.size() > index)
	{
		//do no thing
	}
	else
 	{
		for (size_t i = m_Page.size(); i <= index; i++)
		{
			m_Page.emplace_back();
		}
	}
	if (m_Page[index].m_pContent)
	{
		//do no thing
	}
	else
	{
		m_Page[index].Allocate(m_PageSize);
	}
	return &m_Page[index].m_pContent[address % m_PageSize];
}

LVM::AddressType LVM::MemoryManager::GetPageSize()
{
	return m_PageSize;
}

LVM::Argument LVM::SetMemoryAddress(const std::vector<AddressType> &addrs, bool if_last_jump)
{
	if (addrs.size())
	{
		size_t size = addrs.size() * (sizeof(AddressType) + 1) - (!if_last_jump ? 1 : 0);
		Byte *ptr = new Byte[size];
		for (size_t i = 0; i < addrs.size(); i++)
		{
			*reinterpret_cast<AddressType *>(ptr + i * (sizeof(AddressType) + 1)) = addrs[i];
		}
		return Argument(ptr, size);
	}
	else
	{
		ThrowError("addrs can not be empty");
		return Argument(new char, 1);
	}
}

LVM::AddressType LVM::GetMemoryAddress(const Argument &arg, MemoryManager &memory_manager)
{
	size_t i = 0;
	AddressType re = 0;
	while (i < arg.m_Size)
	{
		if (i % (sizeof(AddressType) + 1) == 0)
		{
			re += *reinterpret_cast<AddressType *>(arg.m_pContent + i);
			i += sizeof(AddressType);
			continue;
		}
		if (i % (sizeof(AddressType) + 1) == sizeof(AddressType))
		{
			re = memory_manager.GetContent<AddressType>(re);
			i += 1;
			continue;
		}

	}
	return re;
}

LVM::Argument LVM::MemoryAddressArgumentToArgument(const std::vector<MemoryAddressArgument>& maa)
{
	size_t size = maa.size() * sizeof(MemoryAddressArgument);
	void* ptr = new Byte[size];
	memcpy(ptr, maa.data(), size);
	return Argument(ptr, size);
}

std::vector<LVM::MemoryAddressArgument> LVM::ArgumentToMemoryAddressArgument(const Argument & arg)
{
	std::vector<MemoryAddressArgument> re;
	re.resize(arg.m_Size / sizeof(MemoryAddressArgument));
	memcpy(re.data(), arg.m_pContent, arg.m_Size);
	return re;
}
