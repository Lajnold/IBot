#ifndef IRC_SAY_H
#define IRC_SAY_H

#include "CommandHandler.h"
#include "IRCBot.h"
#include "IRC_types.h"

namespace IRC
{
	class Say : public CommandHandler
	{
	public:
		Say(IRCBot *bot, const char command_char);

		void handle(const message_list_type &input);
	};
}

#endif
