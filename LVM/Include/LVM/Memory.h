#include "stdafx.h"

namespace LVM
{
	typedef uint64_t AddressType;

	class MemoryPage
	{
	public:
		static const AddressType MemoryPageDefaultSize = 0XFFFFF;

		MemoryPage();
		~MemoryPage();
		void Allocate(AddressType size = MemoryPageDefaultSize);
		void Release();
		AddressType GetSize();
	private:
		Byte* m_pContent;
		AddressType m_Size;
	};

	class MemoryManager
	{
	public:
		MemoryManager(AddressType size = MemoryPage::MemoryPageDefaultSize);
	private:
		AddressType m_PageSize;
		std::vector<MemoryPage> m_Pages;
	};
}