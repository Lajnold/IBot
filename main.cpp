#include "IRCBot.h"

int main(int argc, char *argv[])
{
	IRC::IRCBot bot("127.0.0.1", 9997, "#qwer", '!');
	bot.run();
}
