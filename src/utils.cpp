#include <string>

#include "utils.h"
#include "IRC_types.h"

void IRC::utils::split_string(IRC::core::packet_t &out, const std::string &input, const std::string &delimiter)
{
	std::string::size_type first = 0, second = input.find(delimiter);

	out.push_back(input.substr(first, second));

	while(second != std::string::npos)
	{
		first = second + 1;
		second = input.find(delimiter, first);
		out.push_back(input.substr(first, second - first));
	}
}

std::string IRC::utils::string_to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}
