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

	class SingleChar : public TransitionPattern
	{
		friend class TransitionPatternManager;

	public:
		SingleChar( const char c );

		const Type GetType() override;

		bool Accept( const char c ) override;

	private:
		const char acceptableChar;
	};

	class Range : public TransitionPattern
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

	class Epsilon : public TransitionPattern
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

		~TransitionPatternManager();

		SingleChar *GetSingleCharPattern( const char c );

		Range *GetRangeCharPattern( std::vector<std::pair<char, char>> ranges, bool isComplement );

		Epsilon *GetEpsilonPattern() const;

	private:
		std::vector<SingleChar *> SinglePool;
		std::vector<Range *> RangePool;
		Epsilon *const _EPSILON = new Epsilon();
	};

	extern const TransitionPatternManager TPManager = TransitionPatternManager();

	class NFAState;

	using NFAS_TT = std::map<TransitionPattern *, std::set<NFAState *>>; // NFA State Transition Table
	class NFAState
	{
	public:
		NFAState() = default;

		NFAState( const NFAS_TT transition );

		~NFAState() = default;

		NFAS_TT &GetTransitionTable();

	private:
		NFAS_TT transition;
	};

	class DFAState;

	using DFAS_TT = std::map<TransitionPattern *, DFAState *>; // DFA State Transition Table
	class DFAState
	{
	public:
		DFAState() = default;

		DFAState( const std::map<TransitionPattern *, DFAState *> transition );

		~DFAState() = default;

		DFAS_TT &GetTransitionTable();

	private:
		DFAS_TT transition;
	};

	class NFA
	{
		// NOTE: the following NFA operation functions' arguments should be one of the NFAs, whose terminal state has no transition

		friend NFA *NFA_Cat( NFA *m, NFA *n );

		friend NFA *NFA_Or( NFA *m, NFA *n );

		friend NFA *NFA_Kleene( NFA *m );

	public:
		NFA();

		NFA( NFAState *start, NFAState *terminal );

		~NFA() = default;

		NFAState *GetStartState();

		NFAState *GetTerminalState();

	private:
		NFAState *start, *terminal;
	};
}
