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
#include <memory>

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

	using NFAS_TT = std::map<TransitionPattern *, std::set<std::shared_ptr<NFAState>>>; // NFA State Transition Table
	class NFAState
	{
	public:
		NFAState() = default;

		NFAState( const NFAS_TT &transition );

		~NFAState() = default;

		NFAS_TT &GetTransitionTable();

	private:
		NFAS_TT transition;
	};

	using NFAStatePointer = std::shared_ptr<NFAState>;
	using NFAStateSet = std::set<NFAStatePointer>;

	NFAStatePointer NewNFAState();

	NFAStatePointer NewNFAState( NFAS_TT tt );

	class NFA
	{
	public:
		NFA();

		NFA( const NFAStatePointer start, const NFAStateSet &terminals );

		NFAStatePointer GetStartState();

		NFAStateSet &GetTerminalStates();

		bool Match( const std::string &str );

	private:
		NFAStatePointer Start;
		NFAStateSet Terminals;
	};

	NFAStateSet EpsilonClosure( NFAStatePointer start );

	NFAStateSet EpsilonClosure( const NFAStateSet &starts );

	NFAStateSet Move( const NFAStateSet &starts, const char c );

	const NFA ConstructSingleCharNFA( const char c );

	const NFA ConstructPureStringNFA( const std::string &string );

	const NFA NFAOr( NFA &m, NFA &n );

	const NFA NFACat( NFA &m, NFA &n );

	const NFA NFAKleene( NFA &m );
}
