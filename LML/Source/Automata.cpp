
#include <LML/Lexical/Automata.h>

using namespace LML::Lexical;

SingleChar::SingleChar( const char32_t c ) : acceptableChar( c ) {}

const TransitionPattern::Type SingleChar::GetType()
{
	return Type::SINGLE;
}

bool SingleChar::Accept( const char32_t c )
{
	return c == acceptableChar;
}

const TransitionPattern::Type Epsilon::GetType()
{
	return Type::EPSILON;
}

bool Epsilon::Accept( const char32_t c )
{
	return true;
}

Range::Range( std::initializer_list<std::pair<char32_t, char32_t>> ranges ) : ranges( ranges ) {}

const TransitionPattern::Type Range::GetType()
{
	return Type::RANGE;
}

bool Range::Accept( const char32_t c )
{
	for (auto &range:ranges)
	{
		if (c >= range.first && c <= range.second)
		{
			return true;
		}
	}
	return false;
}
