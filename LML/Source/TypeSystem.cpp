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
#include "LML/stdafx.h"
#include "LML/TypeSystem.h"

using namespace LML;
using namespace std;

LML::TypeId::TypeId()
{

}

LML::TypeId::TypeId(const string & name)
{
	m_Content = name;
}

void LML::TypeId::SetAsFunction(const string & name)
{
	m_Content.clear();
	AddProperty(string(&DefaultTypeName::FunctionTypeName));
	AddElement(name);
}

void LML::TypeId::SetAsClass(const string & name)
{
	m_Content.clear();
	AddProperty(string(&DefaultTypeName::ClassTypeName));
	AddElement(name);
}

string LML::TypeId::GetContent()const
{
	return m_Content;
}

LML::TypeId::TypeConcept LML::TypeId::GetConcept() const
{
	if (m_Content.empty())
		ThrowError("This typeid need initialization");
	if (m_Content[0] == DefaultTypeName::FunctionTypeName)
		return TypeConcept::FunctionConcept;
	if (m_Content[0] == DefaultTypeName::ClassTypeName)
		return TypeConcept::ClassConcept;
	return TypeConcept::VariableConcept;
}

string LML::TypeId::GetClassName() const
{
	if (m_Content.empty() || GetConcept() != TypeConcept::ClassConcept)
		ThrowError("This type is not class");
	string name;
	bool if_record = false;
	for (int i = 1; i < m_Content.size(); i++)
	{
		if (m_Content[i] == '[')
			if_record = true;
		if (m_Content[i] == ']')
		{
			if_record = false;
			break;
		}
		if (if_record)
			name += m_Content[i];
	}
	return name;
}

bool LML::TypeId::operator==(const TypeId & other)const
{
	return (m_Content==other.m_Content);
}

void LML::TypeId::AddElement(const string & name)
{
	if (m_Content.size() == 0)
	{
		m_Content = name;
	}
	else
	{
		m_Content += "[" + name + "]";
	}
}

void LML::TypeId::AddProperty(const string & name)
{
	if (m_Content.size() == 0)
	{
		m_Content = name;
	}
	else
	{
		m_Content += name;
	}
}

bool LML::operator<(const TypeId & t1, const TypeId & t2)
{
	return t1.GetContent() < t2.GetContent();
}

LML::TypeId LML::GetFunctionType(const TypeId & retype, const vector<TypeId>& option_type)
{
	TypeId re;
	string buffer;
	buffer += "[" + retype.GetContent() + "]";
	for (auto i : option_type)
	{
		buffer += "[" + i.GetContent() + "]";
	}
	re.SetAsFunction(buffer);
	return re;
}

LML::TypeId LML::GetClassType(const string & class_name)
{
	TypeId re;
	re.SetAsClass(class_name);
	return re;
}

LML::TypeManager::TypeManager()
{

}

LML::TypeManager::~TypeManager()
{

}

void LML::TypeManager::InsertType(const TypeInformation & type)
{
	m_TypeInformation.insert(std::make_pair(type.m_TypeId, type));
}

void LML::TypeManager::InsertType(const FunctionTypeInformation & type)
{
	m_FunctionTypeInformation.insert(std::make_pair(type.m_TypeId, type));
}
