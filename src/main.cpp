#include "IRCBot.h"
#include "BotOptions.h"

int main(int argc, char *argv[])
{
	IRC::core::BotOptions options;
	
	if(!IRC::core::parse_commandline(argc, argv, options))
		return 0;

	IRC::core::IRCBot bot(options);
	bot.run();
}
