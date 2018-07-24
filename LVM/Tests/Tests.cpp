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
	auto& test = mm.GetContent<AddressType>({ MemoryAddressArgument(1 << 30) });
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
		Argument arg1 = MemoryAddressArgumentToArgument({ MemoryAddressArgument(0) });
		SECTION("save to argument")
		{
			REQUIRE(arg1.As<MemoryAddressArgument>() == MemoryAddressArgument(0));
		}
		SECTION("read from argument")
		{
			auto memory_address1 = mm_1.GetContent<AddressType>(ArgumentToMemoryAddressArgument(arg1));
			REQUIRE(memory_address1 == 0);
		}
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
	std::vector<Command> commands{ Command(*AssignCommand.m_pCommandType,{MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),Argument(new int(1),1)}) };
	SaveCommandsToFile(file, commands);
	file.close();
	VirtualMachine vm;
	vm.RunFromFile("test_assign.lll");
	REQUIRE(vm.GetMemoryManager().GetContent<int>({ MemoryAddressArgument(0) }) == 1);
}
TEST_CASE("Test Copy Move Goto Jump_If", "[LVMTest][Command]")
{
	fstream file("test_cmgj.lll", ios::out | ios::binary);
	std::vector<Command> commands
	{
		Command(*AssignCommand.m_pCommandType,{ MemoryAddressArgumentToArgument({ MemoryAddressArgument(0) }),Argument(new bool(true)) }),
		Command(*CopyCommand.m_pCommandType,{ MemoryAddressArgumentToArgument({ MemoryAddressArgument(4) }),MemoryAddressArgumentToArgument({ MemoryAddressArgument(0) }),Argument(new uint64_t(sizeof(bool)))}),
		Command(*MoveCommand.m_pCommandType,{ MemoryAddressArgumentToArgument({ MemoryAddressArgument(8) }),MemoryAddressArgumentToArgument({ MemoryAddressArgument(0) }),Argument(new uint64_t(sizeof(bool))) }),
		Command(*AssignCommand.m_pCommandType,{ MemoryAddressArgumentToArgument({ MemoryAddressArgument(12) }),Argument(new int32_t(0)) }),
		Command(*JumpIfCommand.m_pCommandType,{Argument(new uint64_t(7)),MemoryAddressArgumentToArgument({ MemoryAddressArgument(4) }) }),
		Command(*AssignCommand.m_pCommandType,{ MemoryAddressArgumentToArgument({ MemoryAddressArgument(12) }),Argument(new int32_t(1))}),
		Command(*GotoCommand.m_pCommandType,{Argument(new uint64_t(0))})
	};
	SaveCommandsToFile(file, commands);
	file.close();
	VirtualMachine vm;
	vm.RunFromFile("test_cmgj.lll");
	REQUIRE(vm.GetMemoryManager().GetContent<bool>({ MemoryAddressArgument(4) }) == true);
	REQUIRE(vm.GetMemoryManager().GetContent<bool>({ MemoryAddressArgument(8) }) == true);
	REQUIRE(vm.GetMemoryManager().GetContent<bool>({ MemoryAddressArgument(0) }) == false);
	REQUIRE(vm.GetMemoryManager().GetContent<int32_t>({ MemoryAddressArgument(12) }) == 0);
}

void TestFunc1(int a, int b)
{
	std::cout << a + b << std::endl;
}

TEST_CASE("Test DLL", "[LVMTest][LVMSDK]") {
#ifdef _WIN32
    DLL test_dll("./TestLib/Release/TestLib.dll");
#endif

#ifdef __APPLE__
    DLL test_dll("./TestLib/libTestLib.dylib");
#endif

#ifdef __linux__
    DLL test_dll("./TestLib/libTestLib.so");
#endif

    using TestDLLFunctionType = void (*)();
    TestDLLFunctionType test_func = (TestDLLFunctionType) test_dll.GetAddress("greet");
    test_func();
}