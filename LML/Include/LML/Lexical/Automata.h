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
	class TransitionPatternManager;

	class TransitionPattern
	{
	public:
		enum class Type : int8_t
		{
			SINGLE, RANGE, EPSILON
		};

		virtual const Type GetType() = 0;

		virtual bool Accept( const char c ) = 0;
	};

	class SingleChar : TransitionPattern
	{
		friend class TransitionPatternManager;

	public:
		SingleChar( const char c );

		const Type GetType() override;

		bool Accept( const char c ) override;

	private:
		const char acceptableChar;
	};

	class Range : TransitionPattern
	{
		friend class TransitionPatternManager;

	public:
		Range( std::vector<std::pair<char, char>> ranges, bool isComplement );

		const Type GetType() override;

		bool Accept( const char c ) override;

	private:
		const std::vector<std::pair<char, char >> ranges;
		const bool compMode;
	};

	class Epsilon : TransitionPattern
	{
	public:
		Epsilon() = default;

		const Type GetType() override;

		bool Accept( const char c ) override;
	};

	class TransitionPatternManager
	{
	public:
		TransitionPatternManager() = default;

		~TransitionPatternManager() = default;

		SingleChar *GetSingleCharParttern( const char c );

		Range *GetRangeCharParttern( std::vector<std::pair<char, char>> ranges, bool isComplement );

		Epsilon *GetEpsilonParttern();

	private:
		std::vector<SingleChar *> SinglePool;
		std::vector<Range *> RangePool;
		Epsilon *const _EPSILON = new Epsilon();
	};

	class NFAState
	{
	public:
		NFAState( const std::map<TransitionPattern *, std::set<NFAState *>> transition );

	private:
		const std::map<TransitionPattern, std::set<NFAState *>> transition;
	};

}
