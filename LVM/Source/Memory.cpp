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

void LVM::MemoryPage::Allocate( AddressType size )
{
	if (m_pContent)
	{
		ThrowError( "can not allocate for a non-empty memory page" );
		return;
	}
	m_Size = size;
	m_pContent = new Byte[size];
	memset( m_pContent, 0, m_Size * sizeof( Byte ));
}

void LVM::MemoryPage::Release()
{
	if (!m_pContent)
	{
		ThrowError( "can not release a empty memory page" );
		return;
	}
	delete[] m_pContent;
	m_pContent = nullptr;
}

LVM::AddressType LVM::MemoryPage::GetSize()
{
	return m_Size;
}

LVM::MemoryManager::MemoryManager( AddressType size )
{
	m_PageSize = size;
}

LVM::Byte *LVM::MemoryManager::GetMemoryContent( AddressType address, SizeType size )
{
	uint32_t index = address / m_PageSize;
	if (( address + size - 1 ) / m_PageSize != index)        //size safe check
	{
		ThrowError( "the size is larger that the memory left in the memory page" );
		return nullptr;
	}
	if (m_Page.size() > index)
	{
		//do no thing
	} else
	{
		for (SizeType i = m_Page.size(); i <= index; i++)
		{
			m_Page.emplace_back();
		}
	}
	if (m_Page[index].m_pContent)
	{
		//do no thing
	} else
	{
		m_Page[index].Allocate( m_PageSize );
	}
	return &m_Page[index].m_pContent[address % m_PageSize];
}

LVM::Byte *LVM::MemoryManager::GetLinkContent( AddressType address )
{
	auto iter = m_Link.find( address );
	if (iter != m_Link.end())
	{
		return (Byte *) iter->second();
	} else
	{
		ThrowError( "do not find the link which called " + std::to_string( address ));
		return nullptr;
	}
}

LVM::Byte *LVM::MemoryManager::GetContent( const std::vector<MemoryAddressArgument> &maa, SizeType size )
{
	Byte *re = nullptr;
	AddressType address = 0;
	bool if_link = false;
	for (auto &i : maa)
	{
		address += i.m_Content;
		switch (i.m_Type)
		{
			case MemoryAddressArgumentType::Default:
				break;
			case MemoryAddressArgumentType::Jump:
				if (if_link)
					address = GetLinkContent<AddressType>( address );
				else
					address = GetMemoryContent<AddressType>( address );
				break;
			case MemoryAddressArgumentType::Memory:
				if_link = false;
				break;
			case MemoryAddressArgumentType::Link:
				if_link = true;
				break;
			default:
				ThrowError( "unkown MemoryAddressArgumentType" );
				break;
		}
	}
	if (if_link)
		re = GetLinkContent( address );
	else
		re = GetMemoryContent( address, size );
	return re;
}

void LVM::MemoryManager::AddLink( AddressType address, const MemoryLink &memory_link )
{
	auto iter = m_Link.find( address );
	if (iter != m_Link.end())
	{
		ThrowError( "can not add the same address's link twice" );
		return;
	}
	m_Link[address] = memory_link;
}

LVM::AddressType LVM::MemoryManager::GetPageSize()
{
	return m_PageSize;
}

LVM::MemoryAddressArgument::MemoryAddressArgument( AddressType address )
{
	m_Content = address;
}

LVM::MemoryAddressArgument::MemoryAddressArgument( AddressType address, MemoryAddressArgumentType type )
{
	m_Content = address;
	m_Type = type;
}

bool LVM::operator==( const MemoryAddressArgument &maa1, const MemoryAddressArgument &maa2 )
{
	return ( maa1.m_Content == maa2.m_Content && maa1.m_Type == maa2.m_Type );
}

LVM::Argument LVM::MemoryAddressArgumentToArgument( const std::vector<MemoryAddressArgument> &maa )
{
	SizeType size = maa.size() * ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType ));
	void *ptr = new Byte[size];
	for (size_t i = 0; i < maa.size(); i++)
	{
		memcpy((Byte *) ptr + i * ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType )), &maa[i].m_Content,
			   sizeof( AddressType ));
		memcpy((Byte *) ptr + i * ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType )) +
			   sizeof( AddressType ), &maa[i].m_Type, sizeof( MemoryAddressArgumentType ));
	}
	return Argument( ptr, size );
}

std::vector<LVM::MemoryAddressArgument> LVM::ArgumentToMemoryAddressArgument( const Argument &arg )
{
	std::vector<MemoryAddressArgument> re;
	re.resize( arg.m_Size / ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType )));
	for (size_t i = 0; i < re.size(); i++)
	{
		memcpy( &re[i].m_Content,
				(Byte *) arg.m_pContent + i * ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType )),
				sizeof( AddressType ));
		memcpy( &re[i].m_Type,
				(Byte *) arg.m_pContent + i * ( sizeof( AddressType ) + sizeof( MemoryAddressArgumentType )) +
				sizeof( AddressType ), sizeof( MemoryAddressArgumentType ));
	}
	return re;
}
