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

LVM::Byte* LVM::MemoryManager::operator[](AddressType address)
{
	uint32_t index = address / m_PageSize;
	if(m_Pages.size()>index)
	{
		//do no thing
	}
	else
	{
		for (size_t i = m_Pages.size(); i <= index; i++)
		{
			m_Pages.emplace_back();
		}
	}
	if (m_Pages[index].m_pContent)
	{
		//do no thing
	}
	else
	{
		m_Pages[index].Allocate(m_PageSize);
	}
	return &m_Pages[index].m_pContent[address % m_PageSize];
}

LVM::AddressType LVM::MemoryManager::GetPageSize()
{
	return m_PageSize;
}

LVM::Argument LVM::SetMemoryAddress(const std::vector<AddressType> &addrs)
{
	if(addrs.size())
	{
		size_t size = addrs.size() * (sizeof(AddressType) + 1);
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
		return Argument(new char,1);
	}
}

LVM::AddressType LVM::GetMemoryAddress(Argument &arg,MemoryManager& memory_manager)
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
			re = *reinterpret_cast<AddressType *>(memory_manager[re]);
			i += 1;
			continue;
		}
		
	}
	return re;
}