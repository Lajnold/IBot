#include <string>
#include <cstring>

#include "Say.h"
#include "IRCBot.h"
#include "utils.h"

namespace IRC
{
	Say::Say(IRCBot *bot, const char command_char) : CommandHandler(bot, command_char)
	{

	}

	void Say::handle(const message_list_type &input)
	{
		std::string owner = strtolower(bot->get_owner());
		if((owner.empty() || owner == strtolower(get_user(input))) && is_command(input, "say"))
		{
			std::string message = get_message(input);
			bot->say(message.substr(5));
		}
	}
}
