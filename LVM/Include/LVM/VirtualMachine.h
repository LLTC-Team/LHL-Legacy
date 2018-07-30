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

#include "LVM/stdafx.h"
#include "LVM/Memory.h"
#include "LVM/Command.h"

namespace LVM
{
	class VirtualMachine
	{
	public:
		/*
		Thread must be create by a VirtualMechine object.
		When a Thread object call it's Run method,it will add itself's pointer to the VirtualMechine object.
		NOTICE:Thread only can be create by `new`.Because the VirtualMechine control it's life.The Threads will be release when VirtualMechine release.
		*/
		struct Thread
		{
		public:
			friend class VirtualMachine;

			void Run( uint64_t start_command_index, VirtualMachine &vm );

			void WaitUntilEnd();

			unsigned int GetId();

			~Thread();

			uint64_t m_CommandRunIndex;
		private:
			Thread() = default;

			std::thread m_Thread;
			std::thread::id m_Id;
			AddressType m_StackTopAddress = 0;
		};

		friend struct Thread;
	public:
		friend struct CommandType;

		VirtualMachine();

		~VirtualMachine();

		void RunFromFile( const std::string &filename );

		void RunFromMemory( const std::vector<Command> commands );

		void AddThread( uint64_t start_command_index = 0 );

		void WaitUntilAllThreadEnd();

		MemoryManager &GetMemoryManager();

		void SetCommandRunIndex( uint64_t index );

	private:
		/*
		before call Run()
		m_CommandContainer must have commands
		*/
		void Run();

	private:
		std::vector<Command> m_CommandContainer;
		MemoryManager m_MemoryManager;
		std::map<std::thread::id, Thread *> m_Thread;
		std::mutex m_Mutex;
	};
}