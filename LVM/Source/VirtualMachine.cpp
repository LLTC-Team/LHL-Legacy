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
#include "LVM/stdafx.h"
#include "LVM/VirtualMachine.h"

LVM::VirtualMachine::VirtualMachine()
{
	m_CommandRunIndex=0;
}

LVM::VirtualMachine::~VirtualMachine()
{

}

LVM::MemoryManager& LVM::VirtualMachine::GetMemoryManager()
{
	return m_MemoryManager;
}

void LVM::VirtualMachine::Run(const std::vector<Command> &commands)
{
	while(m_CommandRunIndex<commands.size())
	{
		commands[m_CommandRunIndex].m_Type.m_RunFunction(commands[m_CommandRunIndex],*this);
		m_CommandRunIndex+=1;
	}
}

void LVM::VirtualMachine::RunFromFile(const std::string &filename)
{
	/*
	file struction
	command size
	...command...
	*/
	std::fstream file;
	file.open(filename,std::ios::in|std::ios::binary);
	m_CommandContainer=LoadCommandsFromFile(file);
	file.close();
	Run(m_CommandContainer);
}

void LVM::VirtualMachine::SetCommandRunIndex(uint64_t index)
{
	m_CommandRunIndex = index;
}