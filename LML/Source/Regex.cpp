#include <LML/Lexical/Regex.h>
#include <new>

LML::Lexical::NFABuilder::NFABuilder() : trans(), dest(), nxt(), size( 0 ), current( 1 )
{

}

LML::Lexical::NFABuilder::~NFABuilder()
{
	for (auto entry : trans)
	{
		delete[] entry;
	}
}

void LML::Lexical::NFABuilder::AddTransition( int from, char c, int to )
{
	if (from < 0 || from >= size || to < 0 || to >= size) {
		return;
	}
	int idx = trans[from][c];
	if (idx == 0) {
		
	}
}

int LML::Lexical::NFABuilder::NewState()
{

}

int LML::Lexical::NFABuilder::GetSize()
{
	return 0;
}
