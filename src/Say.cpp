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

void Say::handle(const core::Message& msg)
{
	std::string owner = utils::to_lower(bot->get_owner());
	if (is_command(msg, "say")) {
		User user = msg.get_user();
		if((owner.empty() || owner == utils::to_lower(user.nick)))
		{
			std::string message = get_command_parameter_string(msg);
			if (!message.empty())
			{
				bot->say(message);
			}
		}
	}
}
