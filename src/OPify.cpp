#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "OPify.h"
#include "CommandHandler.h"
#include "IRCBot.h"
#include "IRC_types.h"
#include "utils.h"

IRC::OPify::OPify(core::IRCBot *bot, const char command_char)
	: core::CommandHandler(bot, command_char)
{
	std::ifstream file("auto_op.txt");
	if(!file)
		return;

	std::string nick;
	while(file >> nick)
	{
		auto_op.push_back(utils::to_lower(nick));
	}
}

void IRC::OPify::make_op(const std::string& nick)
{
	bot->send("MODE " + bot->get_channel() + " +o " + nick + "\r\n");
}

bool IRC::OPify::should_auto_op(const std::string& nick)
{
	return utils::contains(auto_op, utils::to_lower(nick));
}

void IRC::OPify::handle(const core::Message& msg)
{
	if(msg.get_type() == IRC::core::JOIN)
	{
		User user = msg.get_user();
		if (should_auto_op(user.nick))
		{
			make_op(user.nick);
		}

		return;
	}

	if(is_command(msg, "opify"))
	{
		User user = msg.get_user();
		std::string owner = utils::to_lower(bot->get_owner());
		if(!owner.empty() && owner != utils::to_lower(user.nick))
		{
			return;
		}

		StringList params = get_command_parameters(msg);
		if(params.empty())
		{
			return;
		}

		for(auto it = params.begin(); it != params.end(); it++)
		{
			make_op(*it);
		}
	}
}
