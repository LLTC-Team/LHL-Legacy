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

LVM::Byte& LVM::MemoryManager::operator [] (AddressType address)
{
	uint32_t index = address / m_PageSize;
	if (m_Pages[index].m_pContent)
	{
		//do no thing
	}
	else
	{
		m_Pages[index].Allocate(m_PageSize);
	}
	return m_Pages[index].m_pContent[address%m_PageSize];
}