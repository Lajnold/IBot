#include "IRCBot.h"
#include "BotOptions.h"

int main(int argc, char *argv[])
{
	IRC::BotOptions options;
	
	if(!IRC::parse_commandline(argc, argv, options))
		return 0;

	IRC::IRCBot bot(options);
	bot.run();
}
