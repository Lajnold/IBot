#ifndef IRC_FINGER_H
#define IRC_FINGER_H

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class DUMIIFinger : public IRC::core::CommandHandler
	{
		void fill_who_list(packet_t &list);
		
	public:
		DUMIIFinger(IRC::core::IRCBot *bot, const char command_char);
		
		void handle(const packet_t &input);
	};
}

#endif
