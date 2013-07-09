#ifndef IRC_SAY_H
#define IRC_SAY_H

#include "CommandHandler.h"
#include "IRCBot.h"
#include "IRC_types.h"

namespace IRC
{
	class Say : public core::CommandHandler
	{
	public:
		Say(core::IRCBot *bot, const char command_char);

		void handle(const core::Message& msg);
	};
}

#endif
