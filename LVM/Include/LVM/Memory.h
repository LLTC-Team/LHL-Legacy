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
		Byte* m_pContent;
		AddressType m_Size;
	};

	class MemoryManager
	{
	public:
		MemoryManager(AddressType size = MemoryPage::MemoryPageDefaultSize);

		Byte& operator [] (AddressType address);
	private:
		AddressType m_PageSize;
		std::vector<MemoryPage> m_Pages;
	};
}