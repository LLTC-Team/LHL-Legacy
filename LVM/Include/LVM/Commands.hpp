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
#include "LVMSDK.h"
#include "DLL.h"

const LVM::DefineCommandType NullCommand(0, "null", {});

const LVM::DefineCommandType TestCommand(255, "test", {}, [](const LVM::Command& command, LVM::VirtualMachine& vm)-> void
{
	std::cout << "test command" << std::endl;
}
);

/*
copy argument2's content to the memory address which equal to argument1
*/
const LVM::DefineCommandType AssignCommand(1, "assign", { true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[1].m_Size), command.m_Argument[1].m_pContent, command.m_Argument[1].m_Size);
});

/*
copy(dst,src,size:uint64_t)
*/
const LVM::DefineCommandType CopyCommand(2, "copy", { true,true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[2].As<uint64_t>()), vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), command.m_Argument[2].As<uint64_t>());
}
);

/*
move(dst,src,size:uint64_t)
*/
const LVM::DefineCommandType MoveCommand(3, "move", { true,true,false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	memcpy(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[0]), command.m_Argument[2].As<uint64_t>()), vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), command.m_Argument[2].As<uint64_t>());
	memset(vm.GetMemoryManager().GetContent(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1]), command.m_Argument[2].As<uint64_t>()), 0, command.m_Argument[2].As<uint64_t>());
}
);

/*
goto code line:uint64_t
*/
const LVM::DefineCommandType GotoCommand(4, "goto", { false },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	vm.SetCommandRunIndex(command.m_Argument[0].As<uint64_t>() - 1);
}
);

/*
jump if the value equal to true
*/
const LVM::DefineCommandType JumpIfCommand(5, "jump_if", { false,true },
	[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
	if (vm.GetMemoryManager().GetContent<bool>(LVM::ArgumentToMemoryAddressArgument(command.m_Argument[1])))
		vm.SetCommandRunIndex(command.m_Argument[0].As<uint64_t>() - 1);
}
);