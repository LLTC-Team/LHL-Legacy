#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <iostream>
#include "LVM/Memory.h"
#include "LVM/Commands.hpp"

using namespace LVM;
using namespace std;

TEST_CASE( "Test MemoryManager", "[LVMTest][MemoryManager]" )
{
	MemoryManager mm;
	auto &test = mm.GetContent<AddressType>( { MemoryAddressArgument( 1 << 30 ) } );
	test = 4;
	auto ptr = &test;
	AddressType t;
	memcpy( &t, ptr, sizeof( AddressType ));
	REQUIRE( t == test );
}

TEST_CASE( "Test MemoryAddress In Argument", "[LVMTest][Argument][MemoryManager]" )
{
	SECTION( "memory address without jumping " )
	{
		MemoryManager mm_1;
		Argument arg1 = MemoryAddressArgumentToArgument( { MemoryAddressArgument( 0 ) } );
		SECTION( "save to argument" )
		{
			REQUIRE( arg1.As<MemoryAddressArgument>() == MemoryAddressArgument( 0 ));
		}
		SECTION( "read from argument" )
		{
			auto memory_address1 = mm_1.GetContent<AddressType>( ArgumentToMemoryAddressArgument( arg1 ));
			REQUIRE( memory_address1 == 0 );
		}
	}
}

TEST_CASE( "Test Command", "[LVMTest][Command]" )
{
	fstream file( "test_cmd.lll", ios::out | ios::binary );
	std::vector<Command> commands{ Command( *TestCommand.m_pCommandType, {} ) };
	SaveCommandsToFile( file, commands );
	file.close();
	VirtualMachine vm;
	vm.RunFromFile( "test_cmd.lll" );
}

TEST_CASE( "Test Assign Command", "[LVMTest][Command]" )
{
	fstream file( "test_assign.lll", ios::out | ios::binary );
	std::vector<Command> commands{ Command( *AssignCommand.m_pCommandType,
											{ MemoryAddressArgumentToArgument( { MemoryAddressArgument( 0 ) } ),
											  Argument( new int( 1 )) } ) };
	SaveCommandsToFile( file, commands );
	file.close();
	VirtualMachine vm;
	vm.RunFromFile( "test_assign.lll" );
	REQUIRE( vm.GetMemoryManager().GetContent<int>( { MemoryAddressArgument( 0 ) } ) == 1 );
}

TEST_CASE( "Test Copy Move Goto Jump_If", "[LVMTest][Command]" )
{
	fstream file( "test_cmgj.lll", ios::out | ios::binary );
	std::vector<Command> commands
			{
					Command( *AssignCommand.m_pCommandType,
							 { MemoryAddressArgumentToArgument( { MemoryAddressArgument( 0 ) } ),
							   Argument( new bool( true )) } ),
					Command( *CopyCommand.m_pCommandType,
							 { MemoryAddressArgumentToArgument( { MemoryAddressArgument( 4 ) } ),
							   MemoryAddressArgumentToArgument( { MemoryAddressArgument( 0 ) } ),
							   Argument( new uint64_t( sizeof( bool ))) } ),
					Command( *MoveCommand.m_pCommandType,
							 { MemoryAddressArgumentToArgument( { MemoryAddressArgument( 8 ) } ),
							   MemoryAddressArgumentToArgument( { MemoryAddressArgument( 0 ) } ),
							   Argument( new uint64_t( sizeof( bool ))) } ),
					Command( *AssignCommand.m_pCommandType,
							 { MemoryAddressArgumentToArgument( { MemoryAddressArgument( 12 ) } ),
							   Argument( new int32_t( 0 )) } ),
					Command( *JumpIfCommand.m_pCommandType,
							 { Argument( new uint64_t( 7 )),
							   MemoryAddressArgumentToArgument( { MemoryAddressArgument( 4 ) } ) } ),
					Command( *AssignCommand.m_pCommandType,
							 { MemoryAddressArgumentToArgument( { MemoryAddressArgument( 12 ) } ),
							   Argument( new int32_t( 1 )) } ),
					Command( *GotoCommand.m_pCommandType, { Argument( new uint64_t( 0 )) } )
			};
	SaveCommandsToFile( file, commands );
	file.close();
	VirtualMachine vm;
	vm.RunFromFile( "test_cmgj.lll" );
	REQUIRE( vm.GetMemoryManager().GetContent<bool>( { MemoryAddressArgument( 4 ) } ));
	REQUIRE( vm.GetMemoryManager().GetContent<bool>( { MemoryAddressArgument( 8 ) } ));
	REQUIRE_FALSE( vm.GetMemoryManager().GetContent<bool>( { MemoryAddressArgument( 0 ) } ));
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 12 ) } ) == 0 );
}


TEST_CASE( "Test MemoryManager Link", "[LVMTest][MemoryManager][Command]" )
{
	AddressType test_address;
	fstream file( "test_link.lll", ios::out | ios::binary );
	std::vector<Command> commands
			{
					Command{ *AssignCommand.m_pCommandType, { MemoryAddressArgumentToArgument(
							{ MemoryAddressArgument( 1, MemoryAddressArgumentType::Link ) } ),
															  Argument( new AddressType( 8 )) }},
					Command{ *CopyCommand.m_pCommandType, { MemoryAddressArgumentToArgument(
							{ MemoryAddressArgument( 1, MemoryAddressArgumentType::Link ),
							  MemoryAddressArgument( 0, MemoryAddressArgumentType::Jump ),
							  MemoryAddressArgument( 0, MemoryAddressArgumentType::Memory ) } ),
															MemoryAddressArgumentToArgument( { MemoryAddressArgument( 1,
																													  MemoryAddressArgumentType::Link ) } ),
															Argument( new uint64_t( sizeof( uint64_t ))) }}
			};
	SaveCommandsToFile( file, commands );
	file.close();
	VirtualMachine vm;
	vm.GetMemoryManager().AddLink( 1, [&test_address]() { return &test_address; } );
	vm.RunFromFile( "test_link.lll" );

	REQUIRE( vm.GetMemoryManager().GetContent<uint64_t>( { MemoryAddressArgument( 8 ) } ) == 8 );
}

TEST_CASE( "Test Math Command", "[LVMTest][Command]" )
{
	fstream file( "test_math.lll", ios::out | ios::binary );
	std::vector<Command> commands{
		Command{*AssignCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 Argument(new int32_t(6))}},
		Command{*AssignCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(4)}),
				 Argument(new int32_t(3))}},
		Command{*IntAddCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(8)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument(
					 {MemoryAddressArgument(4)})}},
		Command{*IntSubCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(12)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntMulCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(16)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntDivCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(20)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntShiftLogicalLeftCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(24)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntShiftArithmeticRightCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(28)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*AssignCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(1024)}),
				 Argument(new int32_t(-100))}},
		Command{*IntShiftLogicalRightCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(32)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(1024)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntAndCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(36)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntOrCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(40)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntXorCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(44)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(4)})}},
		Command{*IntNotCommand.m_pCommandType,
				{MemoryAddressArgumentToArgument({MemoryAddressArgument(48)}),
				 MemoryAddressArgumentToArgument({MemoryAddressArgument(0)})}}};
	SaveCommandsToFile( file, commands );
	file.close();
	VirtualMachine vm;
	vm.RunFromFile( "test_math.lll" );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 8 ) } ) == 9 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 12 ) } ) == 3 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 16 ) } ) == 18 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 20 ) } ) == 2 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 24 ) } ) == 48 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 28 ) } ) == 0 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 32 ) } ) == 536870899 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 36 ) } ) == 2 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 40 ) } ) == 7 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 44 ) } ) == 5 );
	REQUIRE( vm.GetMemoryManager().GetContent<int32_t>( { MemoryAddressArgument( 48 ) } ) == -7 );

	std::cout << "math command test success" << std::endl;
}

TEST_CASE( "Test NativeLibraryManager", "[LVMTest][NativeFunction]" )
{
	NativeLibraryManager nativeLibraryManager( "." );
	using TestFunctionType = void ( * )();
	TestFunctionType test_func = (TestFunctionType) nativeLibraryManager.GetNativeFunction( "TestLib", "greet" );
	test_func();
}