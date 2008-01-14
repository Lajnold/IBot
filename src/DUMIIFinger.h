#ifndef IRC_FINGER_H
#define IRC_FINGER_H

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class DUMIIFinger : public IRC::core::CommandHandler
	{
		void fill_who_list(message_list_type &list);
		
	public:
		DUMIIFinger(IRC::core::IRCBot *bot, const char command_char);
		
		void handle(const message_list_type &input);
	};
}

#endif
