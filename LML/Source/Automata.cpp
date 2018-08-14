#include <utility>

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
#include <queue>

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
	return false;
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

TransitionPatternManager::TransitionPatternManager() : SinglePool(), RangePool(), _EPSILON( new Epsilon()) {}

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

Epsilon *TransitionPatternManager::GetEpsilonPattern()
{
	return _EPSILON;
}

TransitionPatternManager &LML::Lexical::GetTPManager()
{
	static TransitionPatternManager _TPManager;
	return _TPManager;
}

NFAState::NFAState( const std::map<TransitionPattern *, std::set<int32_t>> transition )
		: transition( transition ) {}

NFAS_TT &NFAState::GetTransitionTable()
{
	return transition;
}

NFA::NFA() : Size( 1 ), Start( 0 ), Terminals( { 0 } ), States( { new NFAState() } ) {}

NFA::NFA( const LML::Lexical::NFA &other ) : Size( other.Size ), Start( other.Start ),
											 Terminals( other.Terminals ),
											 States( other.States ) {}

NFA::~NFA()
{
	for (auto ptr : States)
	{
		delete ptr;
	}
}

NFA &NFA::operator=( const LML::Lexical::NFA &other )
{
	if (this != &other)
	{
		Size = other.Size;
		Start = other.Start;
		Terminals = other.Terminals;
		States = other.States;
	}
	return *this;
}

int32_t NFA::GetStartState()
{
	return Start;
}

std::set<int32_t> NFA::GetTerminalStates()
{
	return Terminals;
}

void NFA::AddTerminal( int32_t id )
{
	Terminals.insert( id );
}

void NFA::RemoveTerminal( int32_t id )
{
	Terminals.erase( id );
}

NFAState *NFA::GetState( int32_t id )
{
	if (id >= 0 && id < Size)
	{
		return States[id];
	} else
	{
		return nullptr;
	}
}

int32_t NFA::NewState()
{
	int32_t id = Size;
	++Size;
	States.push_back( new NFAState());
	return id;
}

int32_t NFA::NewState( NFAS_TT tTable )
{
	int32_t id = Size;
	++Size;
	States.push_back( new NFAState( std::move( tTable )));
	return id;
}

int32_t NFA::GetSize()
{
	return Size;
}

std::vector<int32_t> NFA::EpsilonClosure( int32_t start )
{
	return EpsilonClosure( std::vector<int32_t>( { start } ));
}

std::vector<int32_t> NFA::EpsilonClosure( const std::vector<int32_t> &starts )
{
	// Basically a BFS algorithm
	std::vector<int32_t> que( starts );
	std::vector<bool> vis( static_cast<unsigned long>(Size), false );
	for (auto start:starts)
	{
		vis[start] = true;
	}
	for (int i = 0; i < que.size(); i++)
	{
		auto sid = que[i];
		auto state = States[sid];
		for (auto nxt : state->GetTransitionTable()[GetTPManager().GetEpsilonPattern()])
		{
			if (!vis[nxt])
			{
				que.push_back( nxt );
			}
		}
		vis[sid] = true;
	}
	return que;
}

std::vector<int32_t> NFA::Move( const std::vector<int32_t> &starts, char c )
{
	std::set<int32_t> result;
	auto pattern = GetTPManager().GetSingleCharPattern( c );
	for (auto start: starts)
	{
		if (States[start]->GetTransitionTable().count( pattern ) > 0)
		{
			auto nxts = States[start]->GetTransitionTable()[pattern];
			result.insert( nxts.cbegin(), nxts.cend());
		}
	}
	return std::vector<int32_t>( result.cbegin(), result.cend());
}

bool NFA::Match( const std::string &str )
{
	std::vector<int32_t> states = EpsilonClosure( Start );
	for (char c : str)
	{
		states = EpsilonClosure( Move( states, c ));
		if (states.empty())
		{
			return false;
		}
	}
	for (auto s : states)
	{
		if (Terminals.count( s ) > 0)
		{
			return true;
		}
	}
	return false;
}

const NFA LML::Lexical::ConstructSingleCharNFA( const char c )
{
	NFA result;
	result.RemoveTerminal( 0 );
	auto tid = result.NewState();
	result.AddTerminal( tid );
	result.GetState( 0 )->GetTransitionTable()[GetTPManager().GetSingleCharPattern( c )] = { tid };
	return result;
}

const NFA LML::Lexical::ConstructPureStringNFA( const std::string &string )
{
	NFA result;
	result.RemoveTerminal( 0 );
	auto tid = 0;
	for (char c : string)
	{
		auto pid = tid;
		tid = result.NewState();
		result.GetState( pid )->GetTransitionTable()[GetTPManager().GetSingleCharPattern( c )] = { tid };;
	}
	result.AddTerminal( tid );
	return result;
}
