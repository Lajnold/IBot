#include <string>
#include <cstring>

#include "Say.h"
#include "IRCBot.h"
#include "utils.h"

using namespace IRC;
using namespace IRC::core;

Say::Say(::IRCBot *bot, const char command_char) : CommandHandler(bot, command_char)
{

}

void Say::handle(const ::packet_t &input)
{
	std::string owner = utils::string_to_lower(bot->get_owner());
	if((owner.empty() || owner == utils::string_to_lower(get_user(input))) && is_command(input, "say"))
	{
		std::string message = get_message(input);
		if(message.size() > 5)
			bot->say(message.substr(5));
	}
}
