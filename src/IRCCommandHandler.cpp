#include <string>
#include <cassert>

#include "IRCCommandHandler.h"
#include "IRC_types.h"

namespace IRC
{
	IRCCommandHandler::IRCCommandHandler(IRCBot *bot, const char command_character) : command_char(command_character), bot(bot) { }
	
	bool IRCCommandHandler::is_msg(const message_list_type &input)
	{
		return input.size() >= 4 && input[1] == "PRIVMSG";
	}

	bool IRCCommandHandler::is_command(const message_list_type &input)
	{
		return is_msg(input) && input[3][1] == command_char;
	}

	bool IRCCommandHandler::is_command(const message_list_type &input, const std::string &command)
	{
		return is_command(input) && get_command(input) == command;
	}

	std::string IRCCommandHandler::get_channel(const message_list_type &input)
	{
		assert(is_msg(input));
		
		return input[2];
	}

	std::string IRCCommandHandler::get_command(const message_list_type &input)
	{
		assert(is_command(input));
		
		return input[3].substr(2);
	}

	std::string IRCCommandHandler::get_user(const message_list_type &input)
	{
		assert(is_msg(input));
		
		size_t pos = input[0].find("!");
		return input[0].substr(2, pos - 2);
	}
}
