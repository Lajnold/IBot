#include <string>

#include <boost/program_options.hpp>

#include "IRCBot.h"

struct BotOptions
{
	std::string address;
	unsigned short port;
	
	std::string nickname;
	std::string channel;
	
	char command_char;
	
	BotOptions() : address("localhost"), port(9997), nickname("Amon"), channel("#qwer"), command_char('!') { };
};

void parse_commandline(BotOptions &options)
{
	
}

int main(int argc, char *argv[])
{
	BotOptions options;
	
	IRC::IRCBot bot(options.address, options.port, options.nickname, options.channel, options.command_char);
	bot.run();
}
