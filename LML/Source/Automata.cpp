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
#include <LML/Lexical/LexicalError.h>
#include <queue>
#include <list>

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

NFAState::NFAState( const NFAS_TT &transition ) : transition( transition ) {}

NFAS_TT &NFAState::GetTransitionTable()
{
	return transition;
}

std::shared_ptr<NFAState> LML::Lexical::NewNFAState()
{
	return std::make_shared<NFAState>();
}

std::shared_ptr<NFAState> LML::Lexical::NewNFAState( NFAS_TT tt )
{
	return std::make_shared<NFAState>( tt );
}

NFA::NFA() : Start( NewNFAState()), Terminals( { Start } ) {}

NFA::NFA( const NFAStatePointer start, const NFAStateSet &terminals ) : Start( start ), Terminals( terminals ) {}

NFAStatePointer NFA::GetStartState()
{
	return Start;
}

NFAStateSet &NFA::GetTerminalStates()
{
	return Terminals;
}

NFAStateSet LML::Lexical::EpsilonClosure( NFAStatePointer start )
{
	return EpsilonClosure( NFAStateSet{ std::move( start ) } );
}

NFAStateSet LML::Lexical::EpsilonClosure( const NFAStateSet &starts )
{
	// Basically a BFS algorithm
	NFAStateSet result( starts );
	std::vector<NFAStatePointer> que( starts.cbegin(), starts.cend());
	for (int i = 0; i < que.size(); i++)
	{
		auto &ptr = que[i];
		for (auto &nxt : ptr->GetTransitionTable()[GetTPManager().GetEpsilonPattern()])
		{
			if (result.count( nxt ) == 0)
			{
				result.insert( nxt );
				que.push_back( nxt );
			}
		}
	}
	return result;
}

NFAStateSet LML::Lexical::Move( const NFAStateSet &starts, char c )
{
	NFAStateSet result;
	auto pattern = GetTPManager().GetSingleCharPattern( c );
	for (auto &state : starts)
	{
		if (state->GetTransitionTable().count( pattern ) > 0)
		{
			auto nxt = state->GetTransitionTable()[pattern];
			result.insert( nxt.cbegin(), nxt.cend());
		}
	}
	return result;
}

bool NFA::Match( const std::string &str )
{
	NFAStateSet states = EpsilonClosure( Start );
	for (char c : str)
	{
		states = EpsilonClosure( Move( states, c ));
		if (states.empty())
		{
			return false;
		}
	}
	for (auto &s : states)
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
	NFAStatePointer terminal = NewNFAState();
	NFAStatePointer start = NewNFAState( {{ GetTPManager().GetSingleCharPattern( c ), { terminal }}} );
	return NFA( start, { terminal } );
}

const NFA LML::Lexical::ConstructPureStringNFA( const std::string &string )
{
	NFAStatePointer start = NewNFAState(), now = start;
	for (auto c:string)
	{
		NFAStatePointer ptr = NewNFAState();
		now->GetTransitionTable()[GetTPManager().GetSingleCharPattern( c )] = { ptr };
		std::swap( now, ptr );
	}
	return NFA( start, { now } );
}

const NFA LML::Lexical::NFAOr( LML::Lexical::NFA &m, LML::Lexical::NFA &n )
{
	NFAStatePointer start = NewNFAState(
			{{ GetTPManager().GetEpsilonPattern(), { m.GetStartState(), n.GetStartState() }}} );
	NFAStatePointer terminal = NewNFAState();
	for (auto &ts : m.GetTerminalStates())
	{
		ts->GetTransitionTable()[GetTPManager().GetEpsilonPattern()].insert( terminal );
	}
	for (auto &ts : n.GetTerminalStates())
	{
		ts->GetTransitionTable()[GetTPManager().GetEpsilonPattern()].insert( terminal );
	}
	return NFA( start, { terminal } );
}

const NFA LML::Lexical::NFACat( LML::Lexical::NFA &m, LML::Lexical::NFA &n )
{
	for (auto &ts : m.GetTerminalStates())
	{
		ts->GetTransitionTable()[GetTPManager().GetEpsilonPattern()].insert( n.GetStartState());
	}
	return NFA( m.GetStartState(), n.GetTerminalStates());
}

const NFA LML::Lexical::NFAKleene( LML::Lexical::NFA &m )
{
	NFAStatePointer terminal = NewNFAState();
	for (auto &ts : m.GetTerminalStates())
	{
		auto &s = ts->GetTransitionTable()[GetTPManager().GetEpsilonPattern()];
		s.insert( terminal );
		s.insert( m.GetStartState());
	}
	NFAStatePointer start = NewNFAState( {{ GetTPManager().GetEpsilonPattern(), { m.GetStartState(), terminal }}} );
	return NFA( start, { terminal } );
}

#define THROW_ERROR( reason ) throw InvalidRegex( reason, i, regex )

const NFA LML::Lexical::ConstructNFAFromRegex( const std::string &regex )
{
    if(regex.length() == 0){
        return NFA();
    }
    
	// Step 1: Handle OR ('|') operation
    int lpc = 0;
	for (int i = 0; i < regex.length(); i++)
	{
		if (regex[i] == '\\')
		{
			++i;
			continue;
		}
        if(regex[i] == '('){
            ++lpc;
        }
        if(regex[i] == ')'){
            --lpc;
        }
		if (regex[i] == '|' && lpc == 0)
		{
			// Upper level OR found
			NFA nfa1 = ConstructNFAFromRegex( regex.substr( 0, i )), nfa2 = ConstructNFAFromRegex( regex.substr( i + 1 ));
			NFA nfa = NFAOr( nfa1, nfa2 );
			return nfa;
		}
	}

	// Step 2: Transform every char to NFA, handle parentheses and process escape char ('\')
	std::list<NFA> nfa_list;
	for (int i = 0; i < regex.length(); i++)
	{
		if (regex[i] == ')')
		{
			// right parentheses should not appear here
			THROW_ERROR( "Mismatched Parentheses" );
		}
		if (regex[i] == '(')
		{
			// left parentheses found
			int lpc = 1;   // left parentheses count
			for (int j = i + 1; j < regex.length(); j++)
			{
				if (regex[j] == '\\')
				{
					// escape char found
					++j;
					continue;
				}
				if (regex[j] == '(')
				{
					++lpc;
				}
				if (regex[j] == ')')
				{
					--lpc;
					if (lpc == 0)
					{
						// parentheses matched
						NFA nfa = ConstructNFAFromRegex( regex.substr( i + 1, j - i - 1));
						nfa_list.push_back( nfa );
						i = j;
						break;
					}
				}
			}
			if (lpc != 0)
			{
				// more left parentheses count than right
				THROW_ERROR( "Mismatched Parentheses" );
			}
			continue;
		}
		if (regex[i] == '*')
		{
			// handle Kleene closure
			if (nfa_list.empty())
			{
				// no char to kleene
				THROW_ERROR( "Invalid Kleene closure" );
			}
			auto last = nfa_list.end();
            --last;
			*last = NFAKleene( *last );
			continue;
		}
		if (regex[i] == '\\')
		{
			// escape char found
			if (i == regex.length() - 1)
			{
				// cannot find char to escape
				THROW_ERROR( "Bad Escape Char" );
			}
			++i;  // move index to next position
		}
		nfa_list.push_back( ConstructSingleCharNFA( regex[i] ));
	}

	// Step 3: Connect NFA
	auto it = nfa_list.begin();
	NFA nfa = *it;
	for (++it; it != nfa_list.end(); ++it)
	{
		nfa = NFACat( nfa, *it );
	}

	// Finished
	return nfa;
}