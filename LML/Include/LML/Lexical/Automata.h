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
		TransitionPatternManager();

		~TransitionPatternManager();

		SingleChar *GetSingleCharPattern( const char c );

		Range *GetRangeCharPattern( std::vector<std::pair<char, char>> ranges, bool isComplement );

		Epsilon *GetEpsilonPattern();

	private:
		std::vector<SingleChar *> SinglePool;
		std::vector<Range *> RangePool;
		Epsilon *const _EPSILON;
	};

	TransitionPatternManager &GetTPManager();

	class NFAState;

	using NFAS_TT = std::map<TransitionPattern *, std::set<int32_t>>; // NFA State Transition Table
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

	class NFA
	{
	public:
		NFA();

		NFA( const NFA &other );

		~NFA();

		NFA &operator=( const NFA &other );

		int32_t GetStartState();

		std::set<int32_t> GetTerminalStates();

		void AddTerminal( int32_t id );

		void RemoveTerminal( int32_t id );

		NFAState *GetState( int32_t id );

		int32_t NewState();

		int32_t NewState( NFAS_TT tTable );

		int32_t GetSize();

		std::vector<int32_t> EpsilonClosure( int32_t start );

		std::vector<int32_t> EpsilonClosure( const std::vector<int32_t> &starts );

		std::vector<int32_t> Move( const std::vector<int32_t> &starts, char c );

		bool Match( const std::string &str );

	private:
		int32_t Size, Start;
		std::set<int32_t> Terminals;
		std::vector<NFAState *> States;
	};

	const NFA ConstructSingleCharNFA( const char c );

	const NFA ConstructPureStringNFA( const std::string &string );
}
