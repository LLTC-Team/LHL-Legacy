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

#include <LML/Lexical/Automata.h>

using namespace LML::Lexical;

SingleChar::SingleChar( const char c ) : acceptableChar( c ) {}

const TransitionPattern::Type SingleChar::GetType()
{
	return Type::SINGLE;
}

bool SingleChar::Accept( const char c )
{
	return c == acceptableChar;
}

const TransitionPattern::Type Epsilon::GetType()
{
	return Type::EPSILON;
}

bool Epsilon::Accept( const char c )
{
	return true;
}

Range::Range( std::vector<std::pair<char, char>> ranges, bool isComplement ) : ranges( std::move( ranges )),
																			   compMode( isComplement ) {}

const TransitionPattern::Type Range::GetType()
{
	return Type::RANGE;
}

bool Range::Accept( const char c )
{
	for (auto &range:ranges)
	{
		if (c >= range.first && c <= range.second)
		{
			return !compMode;
		}
	}
	return compMode;
}

TransitionPatternManager::~TransitionPatternManager()
{
	for (auto single:SinglePool)
	{
		delete single;
	}
	for (auto range:RangePool)
	{
		delete range;
	}
	delete _EPSILON;
}

SingleChar *TransitionPatternManager::GetSingleCharPattern( const char c )
{
	for (auto single : SinglePool)
	{
		if (single->acceptableChar == c)
		{
			return single;
		}
	}
	auto ptr = new SingleChar( c );
	SinglePool.push_back( ptr );
	return ptr;
}

Range *TransitionPatternManager::GetRangeCharPattern( std::vector<std::pair<char, char>> ranges, bool isComplement )
{
	// Simply ranges
	// Time complexity O(N·log(N) + N) while N = |ranges|
	std::sort( ranges.begin(), ranges.end());
	std::vector<std::pair<char, char>> newRanges;
	std::pair<char, char> tempRange = std::make_pair( 0, 0 );
	for (auto range : ranges)
	{
		if (range.first <= tempRange.second)  // As ranges was sorted, range.first >= tempRange.first must be true
		{
			tempRange.second = std::max( tempRange.second, range.second );
		} else
		{
			newRanges.push_back( tempRange );
			tempRange = range;
		}
	}
	newRanges.push_back( tempRange );

	// Check duplication
	for (auto pattern : RangePool)
	{
		if (pattern->compMode == isComplement && pattern->ranges == newRanges)
		{
			return pattern;
		}
	}

	// Construct new object
	auto ptr = new Range( newRanges, isComplement );
	RangePool.push_back( ptr );
	return ptr;
}

Epsilon *TransitionPatternManager::GetEpsilonPattern() const
{
	return _EPSILON;
}

NFAState::NFAState( const std::map<TransitionPattern *, std::set<NFAState *>> transition )
		: transition( transition ) {}


NFAS_TT &NFAState::GetTransitionTable()
{
	return transition;
}

DFAState::DFAState( const std::map<TransitionPattern *, DFAState *> transition )
		: transition( transition ) {}

DFAS_TT &DFAState::GetTransitionTable()
{
	return transition;
}

NFA::NFA() : start(), terminal()
{
	start->GetTransitionTable()[TPManager.GetEpsilonPattern()].insert( terminal );
}

NFA::NFA( NFAState *start, NFAState *terminal ) : start( start ), terminal( terminal ) {}

NFAState *NFA::GetStartState()
{
	return start;
}

NFAState *NFA::GetTerminalState()
{
	return terminal;
}

NFA *LML::Lexical::NFA_Cat( NFA *m, NFA *n )
{
	auto *result = new NFA( m->GetStartState(), m->GetTerminalState());
	result->GetTerminalState()->GetTransitionTable() = n->GetStartState()->GetTransitionTable();
	result->terminal = n->terminal;
	return result;
}

NFA *LML::Lexical::NFA_Or( NFA *m, NFA *n )
{
	auto *newStart = new NFAState( {{ TPManager.GetEpsilonPattern(), { m->GetStartState() }},
									{ TPManager.GetEpsilonPattern(), { n->GetStartState() }}} );

	auto *newTerminal = new NFAState();
	m->GetTerminalState()->GetTransitionTable()[TPManager.GetEpsilonPattern()].insert( newTerminal );
	n->GetTerminalState()->GetTransitionTable()[TPManager.GetEpsilonPattern()].insert( newTerminal );

	return new NFA( newStart, newTerminal );
}

NFA *LML::Lexical::NFA_Kleene( NFA *m )
{
	auto *newTerminal = new NFAState();
	auto *newStart = new NFAState( {{ TPManager.GetEpsilonPattern(), { m->GetStartState() }},
									{ TPManager.GetEpsilonPattern(), { newTerminal }}} );

	auto &epsilonTransitions = m->GetTerminalState()->GetTransitionTable()[TPManager.GetEpsilonPattern()];
	epsilonTransitions.insert( m->GetStartState());
	epsilonTransitions.insert( newTerminal );

	return new NFA( newStart, newTerminal );
}
