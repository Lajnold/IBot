#include "IRCBot.h"

int main()
{
	IRC::IRCBot bot("127.0.0.1", 9997, "#qwer", '!');
	bot.run();
}
