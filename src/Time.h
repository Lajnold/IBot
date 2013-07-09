#ifndef IRC_TIME_H
#define IRC_TIME_H

#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class Time : public core::CommandHandler
	{
		
		std::string get_time_string();
		
	public:
	
		Time(core::IRCBot *bot, const char command_char);
		
		void handle(const core::Message& msg);
	};
}

#endif
