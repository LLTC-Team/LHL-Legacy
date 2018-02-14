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
#include "stdafx.h"
#include "Command.h"

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