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
#include <set>

namespace LML::Lexical
{
	class TransitionPattern
	{
	public:
		enum class Type : int8_t
		{
			SINGLE, RANGE, EPSILON
		};

		virtual const Type GetType() = 0;

		virtual bool Accept( const char32_t c ) = 0;
	};

	class SingleChar : TransitionPattern
	{
	public:
		SingleChar( const char32_t c );

		const Type GetType() override;

		bool Accept( const char32_t c ) override;

	private:
		const char32_t acceptableChar;
	};

	class Range : TransitionPattern
	{
	public:
		Range( std::initializer_list<std::pair<char32_t, char32_t>> ranges );

		const Type GetType() override;

		bool Accept( const char32_t c ) override;

	private:
		const std::vector<std::pair<char32_t, char32_t >> ranges;
	};

	class Epsilon : TransitionPattern
	{
	public:
		Epsilon() = default;

		const Type GetType() override;

		bool Accept( const char32_t c ) override;
	};

	extern const Epsilon EPSILON_PATTERN = Epsilon();

	class NFAState
	{
	public:
		NFAState( const std::map<TransitionPattern, std::set<NFAState *>> transition );

	private:
		const std::map<TransitionPattern, std::set<NFAState *>> transition;
	};

}
