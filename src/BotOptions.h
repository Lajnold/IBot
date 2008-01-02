#ifndef IRC_BOTOPTIONS_H
#define IRC_BOTOPTIONS_H

#include <string>

namespace IRC 
{
	struct BotOptions
	{
		std::string address;
		unsigned short port;
		
		std::string nickname;
		std::string channel;
		std::string owner;
		
		char command_char;
	};

	bool parse_commandline(int argc, char *argv[], BotOptions &options);
}

#endif
