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
		Byte* m_pContent;
		AddressType m_Size;
	};

	class MemoryManager
	{
	public:
		MemoryManager(AddressType size = MemoryPage::MemoryPageDefaultSize);

		Byte* operator [] (AddressType address);

		template<typename T>
		T& GetContentByAddress(AddressType address)
		{
			return *(reinterpret_cast<T*>((*this)[address]));
		}
		AddressType GetPageSize();
	private:
		AddressType m_PageSize;
		std::vector<MemoryPage> m_Pages;
	};

	/*
	for test to use
	*/
	Argument SetMemoryAddress(const std::vector<AddressType>& addrs);
	
	/*
	Get Memory Address From Argument.
	Include Memory Address Jumping
	*/
	AddressType GetMemoryAddress(Argument& arg,MemoryManager& memory_manager);
}