#include <string>
#include <ctime>
#include <cstring>

#include "Time.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	Time::Time(IRCBot *bot, const char command_char) : CommandHandler(bot, command_char) { }
	
	void Time::handle(const message_list_type &input)
	{
		if(is_command(input, "time"))
		{
			std::string message = get_time_string();
			bot->say(message);
		}		
	}
	
	std::string Time::get_time_string()
	{
		char str[256];
		std::time_t t = std::time(NULL);		
		std::tm *tm = std::localtime(&t);
		std::strftime(str, sizeof(str), "Wohoo! It's %A today! %B %d, in the year of %Y, actually. And the time is %H:%M:%S. But seriously, why don't you just look at the computer clock?", tm);
		return std::string(str, str + std::strlen(str));
	}
}
