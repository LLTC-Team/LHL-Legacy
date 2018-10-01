#include <exception>
#include <stdexcept>
#include <string>

namespace LML::Lexical
{
	class InvalidRegex : public std::logic_error
	{

	public:
		explicit InvalidRegex( const std::string &errorinfo, const int32_t pos, const std::string &regex );

		const std::string errorinfo;
		const int32_t Position;
		const std::string Regex;
	};
}