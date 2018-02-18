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
}