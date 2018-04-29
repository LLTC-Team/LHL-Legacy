#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
#include "LVM/Memory.h"
#include "LVM/Commands.hpp"

using namespace LVM;
using namespace std;

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

TEST_CASE("Test MemoryManager", "[LVMTest][MemoryManager]")
{
	MemoryManager mm;
	//*mm[8] = 4;
	auto& test = mm.GetContent<AddressType>(1 << 30);
	test = 4;
	auto ptr = &test;
	AddressType t;
	memcpy(&t, ptr, sizeof(AddressType));
	REQUIRE(t == test);
}

TEST_CASE("Test MemoryAddress In Argument", "[LVMTest][Argument][MemoryManager]")
{
	SECTION("memory address without jumping ")
	{
		MemoryManager mm_1;
		Argument arg1 = SetMemoryAddress({ 0 });
		SECTION("save to argument")
		{
			REQUIRE(arg1.As<AddressType>() == 0);
		}
		SECTION("read from argument")
		{
			auto memory_address1 = GetMemoryAddress(arg1, mm_1);
			REQUIRE(memory_address1 == 0);
		}
	}

	SECTION("memory address with jumping")
	{
		MemoryManager mm_2;
		Argument arg2 = SetMemoryAddress({ 0, 8 });

		auto memory_address2 = GetMemoryAddress(arg2, mm_2);
		REQUIRE(memory_address2 == 8);

		Argument arg3 = SetMemoryAddress({ 0, 16 }, true);

		auto memory_address3 = GetMemoryAddress(arg3, mm_2);
		REQUIRE(memory_address3 == 0);
	}
}
TEST_CASE("Test Command", "[LVMTest][Command]")
{
	fstream file("test_cmd.lll", ios::out | ios::binary);
	std::vector<Command> commands{ Command(*TestCommand.m_pCommandType,{}) };
	SaveCommandsToFile(file, commands);
	file.close();
	VirtualMachine vm;
	vm.RunFromFile("test_cmd.lll");
}
TEST_CASE("Test Assign Command", "[LVMTest][Command]")
{
	fstream file("test_assign.lll", ios::out | ios::binary);
	std::vector<Command> commands{ Command(*AssignCommand.m_pCommandType,{SetMemoryAddress({0}),Argument(new int(1),1)}) };
	SaveCommandsToFile(file, commands);
	file.close();
	VirtualMachine vm;
	vm.RunFromFile("test_assign.lll");
	REQUIRE(vm.GetMemoryManager().GetContent<int>(0) == 1);
}
TEST_CASE("Test Copy Move Goto Jump_If", "[LVMTest][Command]")
{
	fstream file("test_cmgj.lll", ios::out | ios::binary);
	std::vector<Command> commands
	{
		Command(*AssignCommand.m_pCommandType,{ SetMemoryAddress({ 0 }),Argument(new bool(true)) }),
		Command(*CopyCommand.m_pCommandType,{SetMemoryAddress({4}),SetMemoryAddress({0}),Argument(new uint64_t(sizeof(bool)))}),
		Command(*MoveCommand.m_pCommandType,{ SetMemoryAddress({ 8 }),SetMemoryAddress({ 0 }),Argument(new uint64_t(sizeof(bool))) }),
		Command(*JumpIfCommand.m_pCommandType,{Argument(new uint64_t(5)),SetMemoryAddress({4})}),
		Command(*GotoCommand.m_pCommandType,{Argument(new uint64_t(0))})
	};
	SaveCommandsToFile(file, commands);
	file.close();
	VirtualMachine vm;
	vm.RunFromFile("test_cmgj.lll");
	REQUIRE(vm.GetMemoryManager().GetContent<bool>(4) == true);
	REQUIRE(vm.GetMemoryManager().GetContent<bool>(8) == true);
	REQUIRE(vm.GetMemoryManager().GetContent<bool>(0) == false);
}