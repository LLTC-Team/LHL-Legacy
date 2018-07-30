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

void LVM::VirtualMachine::Thread::Run( uint64_t start_command_index, VirtualMachine &vm )
{
	m_CommandRunIndex = start_command_index;
	m_Thread = std::thread( [this, &vm]() {
		m_Id = std::this_thread::get_id();
		vm.m_Mutex.lock();
		vm.m_Thread[m_Id] = this;
		vm.m_Mutex.unlock();
		while (m_CommandRunIndex < vm.m_CommandContainer.size())
		{
			vm.m_CommandContainer[m_CommandRunIndex].m_pType->m_RunFunction( vm.m_CommandContainer[m_CommandRunIndex],
																			 vm );
			m_CommandRunIndex += 1;
		}
	} );
}

void LVM::VirtualMachine::Thread::WaitUntilEnd()
{
	if (m_Thread.joinable())
		m_Thread.join();
}

unsigned int LVM::VirtualMachine::Thread::GetId()
{
	unsigned int re = 0;
	std::stringstream ss;
	ss << m_Id;
	ss >> re;
	return re;
}

LVM::VirtualMachine::Thread::~Thread()
{
	if (m_Thread.joinable())
		m_Thread.join();
}

LVM::VirtualMachine::VirtualMachine()
{
}

LVM::VirtualMachine::~VirtualMachine()
{
	for (auto i : m_Thread)
	{
		if (i.second)
			delete i.second;
	}
}

LVM::MemoryManager &LVM::VirtualMachine::GetMemoryManager()
{
	return m_MemoryManager;
}

void LVM::VirtualMachine::RunFromMemory( const std::vector<Command> commands )
{
	m_CommandContainer = commands;
	Run();
}

void LVM::VirtualMachine::Run()
{
	/*
	add thread memory link
	*/
	m_MemoryManager.AddLink( 0, [&, this]() -> void * {
		return &( m_Thread[std::this_thread::get_id()]->m_StackTopAddress );
	} );
	/*
	add main thread
	wait for main thread end
	*/
	Thread *main_thread = new Thread();
	main_thread->Run( 0, *this );
	main_thread->WaitUntilEnd();
}

void LVM::VirtualMachine::RunFromFile( const std::string &filename )
{
	/*
	file struction
	command size
	...command...
	*/
	std::fstream file;
	file.open( filename, std::ios::in | std::ios::binary );
	m_CommandContainer = LoadCommandsFromFile( file );
	file.close();
	Run();
}

void LVM::VirtualMachine::WaitUntilAllThreadEnd()
{
	for (auto i : m_Thread)
	{
		i.second->WaitUntilEnd();
	}
}

void LVM::VirtualMachine::AddThread( uint64_t start_command_index )
{
	Thread *thread = new Thread();
	thread->Run( start_command_index, *this );
}

void LVM::VirtualMachine::SetCommandRunIndex( uint64_t index )
{
	std::lock_guard<std::mutex> locker( m_Mutex );
	m_Thread[std::this_thread::get_id()]->m_CommandRunIndex = index;
}