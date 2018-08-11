#include <utility>


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

SingleChar *TransitionPatternManager::GetSingleCharParttern( const char c )
{
	for (auto single : SinglePool)
	{
		if (single->acceptableChar == c)
		{
			return single;
		}
	}
	return SinglePool.emplace_back( c );
}

Range *TransitionPatternManager::GetRangeCharParttern( std::vector<std::pair<char, char>> ranges, bool isComplement )
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
	return RangePool.emplace_back( newRanges, isComplement );
}

Epsilon *TransitionPatternManager::GetEpsilonParttern()
{
	return _EPSILON;
}
