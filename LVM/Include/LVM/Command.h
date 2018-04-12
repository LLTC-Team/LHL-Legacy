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

namespace LVM
{
	class VirtualMachine;
	class CommandTypeManager;
	struct Command;

	using CommandFunctionType = std::function<void(const Command&,VirtualMachine &)>;

	using ArgumentModeType = unsigned int;
	
	constexpr ArgumentModeType GetBoolNumber(const std::initializer_list<bool> content)
	{
	    ArgumentModeType re = 0;
	    ArgumentModeType cot = 0;
	    for (auto i : content)
	    {
	        if (i)
	            re |= 1 << cot;
	        cot += 1;
	    }
		return re;
	}

	std::vector<bool> GetBoolByNumber(ArgumentModeType number,size_t size);

	/*返回值使用来描述一个命令的第n个参数是否是地址*/
	constexpr ArgumentModeType GetArgumentMode(const std::initializer_list<bool> content)
	{
	    return GetBoolNumber(content);
	}

	/*
	type_name是这个命令类型的类型名
	index是这个命令类型的序号
	argument_mode使用来描述这个命令类型的第n个参数是否是地址
	func是这种命令类型运行的函数
	*/
	struct CommandType
	{
		CommandType(const std::string &type_name = "NullCommandType", Byte index = 0, ArgumentModeType argument_mode = 0, size_t argument_size = 0, const CommandFunctionType &func = [](const Command&, VirtualMachine &) -> void {});
		std::string m_Name;
		Byte m_Index;
		ArgumentModeType m_ArgumentMode;
		size_t m_ArgumentSize;
		CommandFunctionType m_RunFunction;
	};

	void NewCommandType(const CommandType& command_type);

	CommandTypeManager& GetCommandTypeManager();

	class CommandTypeManager
	{
		public:
			static const Byte MaxCommandTypeIndex=255;

			friend CommandTypeManager& GetCommandTypeManager();
			
			void InsertCommandType(const CommandType& command_type);
			const CommandType* GetCommandTypeByName(const std::string& name);
			const CommandType* GetCommandTypeByIndex(Byte index);
		private:
			CommandTypeManager();

			std::map<std::string,CommandType> m_Content;
			std::array<const CommandType*,MaxCommandTypeIndex> m_IndexList;
	};

	struct DefineCommandType
	{
		DefineCommandType(Byte index, const std::string& name, ArgumentModeType argument_mode = 0, size_t argument_size = 0, CommandFunctionType func = [](const Command&, VirtualMachine &) -> void {});
        CommandType* m_pCommandType;
	};

	/*
	Argument 指令的参数
	*/
	struct Argument
	{
		Byte* m_pContent;
		size_t m_Size;

		Argument() = delete;
		Argument(void* pointer, size_t size);
		Argument(const Argument& arg);
		~Argument();

		Argument& operator = (const Argument& arg);

		template<typename T>
		T& As()
		{
			return *(reinterpret_cast<T*>(m_pContent));
		}
	};

	/*
	Load Argument From File.
	file must open by std::ios::in|std::ios::binary
	*/
	Argument LoadArgumentFromFile(std::fstream& file);
	/*
	Save Argument To File
	file must open by std::ios::out|std::ios::binary
	*/
	void SaveArgumentToFile(std::fstream & file, const Argument& arg);

	/*
	Command 指令
	*/
	struct Command
	{
		const CommandType& m_Type;
		std::vector<Argument> m_Argument;
		
		Command() = delete;
		Command(const CommandType& type, std::vector<Argument> args);
		Command(const Command& c);
	};
	/*
	Load Command From File.
	file must open by std::ios::in|std::ios::binary
	*/
	Command LoadCommandFromFile(std::fstream& file);
	/*
	Save Command To File
	file must open by std::ios::out|std::ios::binary
	*/
	void SaveCommandToFile(std::fstream & file, const Command& cmd);

    std::vector<Command> LoadCommandsFromFile(std::fstream& file);

	void SaveCommandsToFile(std::fstream& file,const std::vector<Command>& commands);
}