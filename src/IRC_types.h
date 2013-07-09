#ifndef IRC_TYPES_H
#define IRC_TYPES_H

#include <vector>
#include <string>

namespace IRC
{
	typedef std::vector<std::string> StringList;

	struct User
	{
		std::string nick;
		std::string host;

		User() {};
		User(std::string nick, std::string host) : nick(nick), host(host) {};
	};
}

#endif
