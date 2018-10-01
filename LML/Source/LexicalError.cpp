
#include <LML/Lexical/LexicalError.h>

using namespace LML::Lexical;

InvalidRegex::InvalidRegex( const std::string& errorinfo, const int32_t pos, const std::string &regex )
		: logic_error( "Invalid Regex" /*TODO: Prettier info*/), errorinfo( errorinfo ), Position( pos ), Regex( regex ) {}
