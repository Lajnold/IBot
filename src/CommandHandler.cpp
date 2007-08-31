#include <string>
#include <cassert>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	CommandHandler::CommandHandler(IRCBot *bot, const char command_character) : command_char(command_character), bot(bot) { }
	
	bool CommandHandler::is_msg(const message_list_type &input)
	{
		return input.size() >= 4 && input[1] == "PRIVMSG";
	}
	
	bool CommandHandler::is_channel_msg(const message_list_type &input)
	{
		return is_msg(input) && get_channel(input) == bot->get_channel();
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
		assert(is_msg(input));
		
		return input[2];
	}

	std::string CommandHandler::get_command(const message_list_type &input)
	{
		assert(is_command(input));
		
		return input[3].substr(2);
	}

	std::string CommandHandler::get_user(const message_list_type &input)
	{
		assert(is_msg(input));
		
		size_t pos = input[0].find("!");
		return input[0].substr(2, pos - 2);
	}
}
