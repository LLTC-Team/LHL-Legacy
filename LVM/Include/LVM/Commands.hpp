/*
Copyright 2018 creatorlxd

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once
#include "stdafx.h"
#include "VirtualMachine.h"
#include "NativeLibraryManager.h"

extern inline const LVM::DefineCommandType NullCommand(0, "null", {});

extern inline const LVM::DefineCommandType TestCommand(255, "test", {}, [](const LVM::Command& command, LVM::VirtualMachine& vm)-> void
{
	std::cout << "test command" << std::endl;
}
);

/*
copy argument2's content to the memory address which equal to argument1
*/
extern inline const LVM::DefineCommandType AssignCommand(1, "assign", { true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[1].m_Size), command.m_Argument[1].m_pContent, command.m_Argument[1].m_Size);
});

/*
copy(dst,src,size:uint64_t)
*/
extern inline const LVM::DefineCommandType CopyCommand(2, "copy", { true,true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[2].As<uint64_t>()), vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), command.m_Argument[2].As<uint64_t>());
}
);

/*
move(dst,src,size:uint64_t)
*/
extern inline const LVM::DefineCommandType MoveCommand(3, "move", { true,true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[2].As<uint64_t>()), vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), command.m_Argument[2].As<uint64_t>());
	memset(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), 0, command.m_Argument[2].As<uint64_t>());
}
);

/*
goto code line:uint64_t
*/
extern inline const LVM::DefineCommandType GotoCommand(4, "goto", { false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.SetCommandRunIndex(command.m_Argument[0].As<uint64_t>() - 1);
}
);

/*
jump if the value equal to true
*/
extern inline const LVM::DefineCommandType JumpIfCommand(5, "jump_if", { false,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	if (vm.GetMemoryManager().GetContent<bool>(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1])))
		vm.SetCommandRunIndex(command.m_Argument[0].As<uint64_t>() - 1);
}
);

/*
iadd(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntAddCommand(6, "iadd", {true,true,true},
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) + vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
isub(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntSubCommand(7, "isub", { true,true,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) - vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
imul(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntMulCommand(8, "imul", { true,true,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) * vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
idiv(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntDivCommand(9, "idiv", { true,true,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) / vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
ishl(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntShiftLogicalLeftCommand(10,"ishl",{ true,true,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) << vm.GetMemoryManager().GetContent<uint32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
isar(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntShiftArithmeticRightCommand(11, "isar", {true, true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) >> vm.GetMemoryManager().GetContent<uint32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
ishr(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntShiftLogicalRightCommand(12, "ishr", {true, true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<uint32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<uint32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) >> vm.GetMemoryManager().GetContent<uint32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
iand(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntAndCommand(13, "iand", {true, true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) & vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
ior(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntOrCommand(14, "ior", {true, true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) | vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
ixor(dst,arg1,arg2)
*/
extern inline const LVM::DefineCommandType IntXorCommand(15, "ixor", {true, true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1])) ^ vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[2]));
}
);

/*
inot(dst,arg1)
*/
extern inline const LVM::DefineCommandType IntNotCommand(16, "inot", {true, true},
	[](const LVM::Command& command,LVM::VirtualMachine& vm)
{
	vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[0])) = ~vm.GetMemoryManager().GetContent<int32_t>(ArgumentToMemoryAddressArgument(command.m_Argument[1]));
}
);