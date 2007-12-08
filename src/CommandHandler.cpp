#include <string>
#include <cassert>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"
#include "utils.h"

namespace IRC
{
	std::string s;
	CommandHandler::CommandHandler(IRCBot *bot, const char command_character) : command_char(command_character), bot(bot) { }	
	
	bool CommandHandler::is_msg(const message_list_type &input)
	{
		return input.size() >= 4 && input[1] == "PRIVMSG";
	}
	
	bool CommandHandler::is_channel_msg(const message_list_type &input)
	{
		return is_msg(input) &&
			strtolower(get_channel(input)) == strtolower(bot->get_channel());
	}

	bool CommandHandler::is_command(const message_list_type &input)
	{
		return is_msg(input) && input[3][1] == command_char;
	}

	bool CommandHandler::is_command(const message_list_type &input, const std::string &command)
	{
		return is_command(input) && get_command(input) == command;
	}

	std::string CommandHandler::get_channel(const message_list_type &input)
	{
		if(!is_msg(input))
			return "";

		return input[2];
	}

	std::string CommandHandler::get_command(const message_list_type &input)
	{
		if(!is_command(input))
			return "";

		return input[3].substr(2);
	}

	std::string CommandHandler::get_message(const message_list_type &input)
	{
		if(!is_msg(input))
			return "";

		std::string message = input[3].substr(1);

		for(message_list_type::const_iterator iter = input.begin() + 4;
		    iter != input.end();
		    iter++)
			message += " " + *iter;

		return message;
	}

	std::string CommandHandler::get_user(const message_list_type &input)
	{
		if(!is_msg(input))
			return "";
		
		size_t pos = input[0].find("!");
		return input[0].substr(2, pos - 2);
	}
}
