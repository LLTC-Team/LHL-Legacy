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
#include "LVM\stdafx.h"
#include "LVM\Command.h"

LVM::CommandType::CommandType(const std::string& type_name,Byte index,ArgumentModeType argument_mode,const CommandFunctionType& func)
:m_Name(type_name),
 m_Index(index),
 m_ArgumentMode(argument_mode),
 m_RunFunction(func)
{
	
}

void LVM::NewCommandType(const CommandType& command_type)
{
	GetCommandTypeManager().InsertCommandType(command_type);
}

LVM::CommandTypeManager& LVM::GetCommandTypeManager()
{
	static CommandTypeManager g_CommandTypeManager;
	return g_CommandTypeManager;
}

void LVM::CommandTypeManager::InsertCommandType(const LVM::CommandType& command_type)
{
	if(m_Content.find(command_type.m_Name)!=m_Content.end())
	{
		ThrowError("can not insert the same command type twice");
		return;
	}
	m_Content.insert(make_pair(command_type.m_Name,command_type));
	m_IndexList[command_type.m_Index]=&m_Content[command_type.m_Name];
}

const LVM::CommandType* LVM::CommandTypeManager::GetCommandTypeByName(const std::string& name)
{
	auto iter=m_Content.find(name);
	if(iter==m_Content.end())
	{
		ThrowError("do not have this type");
		return nullptr;
	}
	
	return &(iter->second);
}

const LVM::CommandType* LVM::CommandTypeManager::GetCommandTypeByIndex(Byte index)
{
	return m_IndexList[index];
}

LVM::CommandTypeManager::CommandTypeManager()
{
	memset(m_IndexList.data(),0,sizeof(m_IndexList));
}

LVM::DefineCommandType::DefineCommandType(Byte index, const std::string& name, ArgumentModeType argument_mode, CommandFunctionType func)
{
	GetCommandTypeManager().InsertCommandType(CommandType(name, index, argument_mode, func));
}

LVM::Argument::Argument(void * pointer, size_t size)
{
	if (!pointer)
	{
		ThrowError("can not make nullptr as a argument");
	}
	m_pContent = new Byte[size];
	m_Size = size;
	memcpy(m_pContent, pointer, size);
}

LVM::Argument::Argument(const Argument& arg)
{
	m_Size = arg.m_Size;
	delete[] m_pContent;
	m_pContent = new Byte[m_Size];
	memcpy(m_pContent, arg.m_pContent, m_Size);
}

LVM::Argument::~Argument()
{
	delete[] m_pContent;
}

LVM::Argument & LVM::Argument::operator=(const Argument & arg)
{
	m_Size = arg.m_Size;
	delete[] m_pContent;
	m_pContent = new Byte[m_Size];
	memcpy(m_pContent, arg.m_pContent, m_Size);
	return *this;
}

LVM::Command::Command(const CommandType & type, std::initializer_list<std::pair<void*, size_t>> args)
	:
	m_Type(type)
{
	for (auto i : args)
	{
		m_Argument.emplace_back(i.first, i.second);
	}
}