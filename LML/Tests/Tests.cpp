#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
#include "LML/Lexical/Automata.h"

TEST_CASE( "Test NFA that only matches one char", "[LML][Lexical][Automata][NFA]" )
{
	LML::Lexical::NFA nfa = LML::Lexical::ConstructSingleCharNFA( 'a' );

	REQUIRE( nfa.Match( "a" ));

	REQUIRE_FALSE( nfa.Match( "b" ));
	REQUIRE_FALSE( nfa.Match( "c" ));
	REQUIRE_FALSE( nfa.Match( "d" ));
	REQUIRE_FALSE( nfa.Match( "e" ));
	REQUIRE_FALSE( nfa.Match( "1" ));
	REQUIRE_FALSE( nfa.Match( "2" ));
	REQUIRE_FALSE( nfa.Match( "0" ));
	REQUIRE_FALSE( nfa.Match( "" ));
	REQUIRE_FALSE( nfa.Match( "1231" ));
	REQUIRE_FALSE( nfa.Match( "afsdfa" ));
}

TEST_CASE( "Test NFA that matches a string", "[LML][Lexical][Automata][NFA]" )
{
	LML::Lexical::NFA nfa = LML::Lexical::ConstructPureStringNFA("aabbccdd");

	REQUIRE(nfa.Match("aabbccdd"));

	REQUIRE_FALSE(nfa.Match("a"));
	REQUIRE_FALSE(nfa.Match("aa"));
	REQUIRE_FALSE(nfa.Match("dd"));
	REQUIRE_FALSE(nfa.Match("cc"));
	REQUIRE_FALSE(nfa.Match("aasdfadsdsafad"));
	REQUIRE_FALSE(nfa.Match("111111111111111111"));
	REQUIRE_FALSE(nfa.Match(""));
	REQUIRE_FALSE(nfa.Match("a132sa1f3as1f"));
	REQUIRE_FALSE(nfa.Match("afsdjs"));
	REQUIRE_FALSE(nfa.Match("00"));
}