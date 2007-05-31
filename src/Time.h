#ifndef IRC_TIME_H
#define IRC_TIME_H

#include "IRCCommandHandler.h"
#include "IRC_types.h"

namespace IRC
{
	class IRCBot;
	
	class Time : public IRCCommandHandler
	{
		
		std::string get_time_string();
		
	public:
	
		Time(IRCBot *bot, const char command_char);
		
		void handle(const message_list_type &input);
	};
}

#endif
