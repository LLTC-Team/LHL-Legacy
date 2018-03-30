#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
#include "LVM/Memory.h"

using namespace LVM;

unsigned int Factorial(unsigned int number)
{
	return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Hello Catch2", "[hello]")
{
	std::cout << "hello catch2" << std::endl;
	std::cout << "LVM test" << std::endl;
	REQUIRE(true);
}

TEST_CASE("Test MemoryManager","[LVMTest][MemoryManager]")
{
	MemoryManager mm;
	*mm[4] = 4;
}

TEST_CASE("Test MemoryAddress In Argument", "[LVMTest][Argument][MemoryManager]")
{
	SECTION("memory address without jumping ")
	{
		MemoryManager mm_1;
		Argument arg1 = SetMemoryAddress({0});
		SECTION("save to argument")
		{
			REQUIRE(arg1.As<AddressType>() == 0);
		}
		SECTION("read from argument")
		{
			auto memory_address1 = GetMemoryAddress(arg1,mm_1);
			REQUIRE(memory_address1 == 0);
		}
	}

	SECTION("memory address with jumping")
	{
		MemoryManager mm_2;
		Argument arg2 = SetMemoryAddress({0, 4});
		//mm_2.GetContentByAddress<AddressType>(4) = 4;
		std::cout<<*(AddressType*)mm_2[4];

		auto memory_address2 = GetMemoryAddress(arg2, mm_2);
		// //test
		std::cout << memory_address2 << std::endl;
	}
}