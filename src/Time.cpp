#include <string>
#include <ctime>
#include <cstring>

#include "Time.h"
#include "IRC_types.h"
#include "IRCBot.h"

using namespace IRC;

Time::Time(core::IRCBot *bot, const char command_char) : CommandHandler(bot, command_char)
{

}
	
void Time::handle(const core::Message& msg)
{
	if(is_command(msg, "time"))
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
