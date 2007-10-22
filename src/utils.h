#ifndef IRC_UTILS_H
#define IRC_UTILS_H

#include <string>

#include "IRC_types.h"

namespace IRC
{
	void split_string(message_list_type &out, const std::string &input, const std::string &delimiter);
	
	std::string strtolower(std::string s);
}

#endif
