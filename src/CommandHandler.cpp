#include <string>
#include <cassert>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"
#include "utils.h"

using namespace IRC::core;

CommandHandler::CommandHandler(IRCBot *bot, const char command_character)
	: command_char(command_character), bot(bot)
{

}	

bool CommandHandler::is_msg(const packet_t &input)
{
	return input.size() >= 4 && input[1] == "PRIVMSG";
}

bool CommandHandler::is_channel_msg(const packet_t &input)
{
	return is_msg(input) &&
		IRC::utils::string_to_lower(get_channel(input)) == IRC::utils::string_to_lower(bot->get_channel());
}

bool CommandHandler::is_command(const packet_t &input)
{
	return is_msg(input) && input[3][1] == command_char;
}

bool CommandHandler::is_command(const packet_t &input, const std::string &command)
{
	return is_command(input) && get_command(input) == command;
}

std::string CommandHandler::get_channel(const packet_t &input)
{
	if(!is_msg(input))
		return "";

	return input[2];
}

std::string CommandHandler::get_command(const packet_t &input)
{
	if(!is_command(input))
		return "";

	return input[3].substr(2);
}

std::string CommandHandler::get_message(const packet_t &input)
{
	if(!is_msg(input))
		return "";

	std::string message = input[3].substr(1);

	for(packet_t::const_iterator iter = input.begin() + 4;
		iter != input.end();
		iter++)
		message += " " + *iter;

	return message;
}

std::string CommandHandler::get_user(const packet_t &input)
{
	if(!is_msg(input))
		return "";
	
	size_t pos = input[0].find("!");
	return input[0].substr(1, pos - 1);
}
