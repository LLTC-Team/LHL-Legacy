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

/*
include type system:
	int
	float
	char
		=>	constant pool
	function
	class
*/
namespace LML
{
	namespace DefaultTypeName
	{
		const std::string VoidTypeName = "v";
		const std::string IntTypeName = "i";
		const std::string FloatTypeName = "f";
		const std::string CharTypeName = "c";
		const char FunctionTypeName = 'F';
		const char ClassTypeName = 'C';
	}

	class TypeId
	{
	public:
		enum class TypeConcept
		{
			VariableConcept,
			FunctionConcept,
			ClassConcept
		};
		TypeId();
		TypeId(const std::string& name);
		void SetAsFunction(const std::string& name);
		void SetAsClass(const std::string& name);
		std::string GetContent()const;
		TypeConcept GetConcept()const;
		std::string GetClassName()const;

		bool operator == (const TypeId& other) const;
		void AddElement(const std::string& name);
		void AddProperty(const std::string& name);
	private:
		std::string m_Content;
	};

	bool operator < (const TypeId& t1, const TypeId& t2);

	namespace DefaultTypeId
	{
		const TypeId VoidType("v");
		const TypeId IntType("i");
		const TypeId FloatType("f");
		const TypeId CharType("c");
	}

	TypeId GetFunctionType(const TypeId& retype, const std::vector<TypeId>& option_type);
	TypeId GetClassType(const std::string& class_name);

	struct TypeInformation
	{
		TypeId m_TypeId;
		uint32_t m_Size = 0;
		std::map<std::string, std::map<TypeId, uint32_t>> m_MemberVariable;
	};

	struct FunctionTypeInformation
	{
		TypeId m_TypeId;
		uint32_t m_Size = 4;
		std::vector<std::pair<TypeId, uint32_t>> m_VariableList;
		uint32_t m_ArgumentSize = 0;
	};

	class TypeManager
	{
	public:
		TypeManager();
		~TypeManager();

		void InsertType(const TypeInformation& type);
		void InsertType(const FunctionTypeInformation& type);

		template<typename T>
		T GetTypeInformation(const TypeId& type_id)
		{
			ThrowError("can only get type information by typeinformation or functiontypeinformation");
		}

		template<>
		TypeInformation GetTypeInformation(const TypeId& type_id)
		{
			auto iter = m_TypeInformation.find(type_id);
			if (iter==m_TypeInformation.end())
			{
				ThrowError("do not find this typeinformation");
				return TypeInformation();
			}
			return iter->second;
		}

		template<>
		FunctionTypeInformation GetTypeInformation(const TypeId& type_id)
		{
			auto iter = m_FunctionTypeInformation.find(type_id);
			if (iter == m_FunctionTypeInformation.end())
			{
				ThrowError("do not find this functiontypeinformation");
				return FunctionTypeInformation();
			}
			return iter->second;
		}
	private:
		std::map<TypeId, TypeInformation> m_TypeInformation;
		std::map<TypeId, FunctionTypeInformation> m_FunctionTypeInformation;
	};

}