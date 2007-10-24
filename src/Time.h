#ifndef IRC_TIME_H
#define IRC_TIME_H

#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class Time : public CommandHandler
	{
		
		std::string get_time_string();
		
	public:
	
		Time(IRCBot *bot, const char command_char);
		
		void handle(const message_list_type &input);
	};
}

#endif
