#ifndef IRC_TIME_H
#define IRC_TIME_H

#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class Time : public IRC::core::CommandHandler
	{
		
		std::string get_time_string();
		
	public:
	
		Time(IRC::core::IRCBot *bot, const char command_char);
		
		void handle(const packet_t &input);
	};
}

#endif
