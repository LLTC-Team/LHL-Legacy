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

const LVM::DefineCommandType NullCommand(0, "null", {});

const LVM::DefineCommandType TestCommand(255, "test", {}, [](const LVM::Command& command, LVM::VirtualMachine& vm)-> void
{
    std::cout<<"test command"<<std::endl;
}
);

/*
copy argument2's content to the memory address which equal to argument1
*/
const LVM::DefineCommandType AssignCommand(1,"assign",{true,false},
[](const LVM::Command& command, LVM::VirtualMachine& vm)
{
    memcpy(vm.GetMemoryManager().GetContent(LVM::GetMemoryAddress(command.m_Argument[0],vm.GetMemoryManager())),command.m_Argument[1].m_pContent,command.m_Argument[1].m_Size);
});