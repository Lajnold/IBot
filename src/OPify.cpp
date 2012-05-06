#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "OPify.h"
#include "CommandHandler.h"
#include "IRCBot.h"
#include "IRC_types.h"
#include "utils.h"

IRC::OPify::OPify(IRC::core::IRCBot *bot, const char command_char)
	: IRC::core::CommandHandler(bot, command_char)
{
	std::ifstream file("auto_op.txt");
	if(!file)
		return;

	std::string nick;
	while(file >> nick)
		auto_op.push_back(utils::string_to_lower(nick));
}

void IRC::OPify::make_op(const std::string nick)
{
	bot->raw_command("MODE " + bot->get_channel() + " +o " + nick);
}

void IRC::OPify::handle(const IRC::core::packet_t &input)
{
	if(input.size() == 3 && input[1] == "JOIN")
	{
		int pos = input[0].find('!');
		std::string nick = utils::string_to_lower(get_user(input));

		if(std::find(auto_op.begin(), auto_op.end(), nick) != auto_op.end())
			make_op(nick);

		return;
	}

	std::string owner = IRC::utils::string_to_lower(bot->get_owner());
	if(!owner.empty() && (owner != IRC::utils::string_to_lower(get_user(input))))
		return;

	if(is_command(input, "opify"))
	{
		std::string params = get_message(input);
		if(params.empty())
			return;

		IRC::core::packet_t names;
		IRC::utils::split_string(names, params, " ");

		std::string channel = bot->get_channel();
		
		for(IRC::core::packet_t::const_iterator iter = names.begin();
			iter != names.end();
			iter++)
			make_op(*iter);
	}
}
