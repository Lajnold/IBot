#include <stdexcept>
#include <string>

#include "IRC_types.h"
#include "IRCBot.h"
#include "utils.h"

#include "CommandHandler.h"

using namespace IRC::core;

CommandHandler::CommandHandler(IRCBot *bot, const char command_character)
	: command_char(command_character), bot(bot)
{

}	

bool CommandHandler::is_channel_message(const Message& msg)
{
	return msg.is_message() &&
		utils::to_lower(msg.get_channel()) == utils::to_lower(bot->get_channel());
}

bool CommandHandler::is_command(const Message& msg)
{
	return msg.is_message() && msg.get_message()[0] == command_char;
}

bool CommandHandler::is_command(const Message& msg, const std::string &command)
{
	return is_command(msg) && get_command(msg) == command;
}

std::string CommandHandler::get_command(const Message& msg)
{
	if(!is_command(msg))
	{
		throw std::logic_error("is not command");
	}

	// The first word, minus the leading command char.
	std::string message = msg.get_message();
	size_t cmd_end = message.find(' ');
	if (cmd_end == std::string::npos)
	{
		return message.substr(1);
	}
	else
	{
		return message.substr(1, cmd_end - 1);
	}
}

IRC::StringList CommandHandler::get_command_parameters(const Message& msg)
{
	if(!is_command(msg))
	{
		throw std::logic_error("is not command");
	}

	std::string param_string = get_command_parameter_string(msg);
	StringList tokenized_params = utils::split_whitespace<StringList>(param_string);
	utils::remove_val(tokenized_params, "");
		
	return tokenized_params;
}

std::string CommandHandler::get_command_parameter_string(const Message& msg)
{
	if(!is_command(msg))
	{
		throw std::logic_error("is not command");
	}

	// The command parameter starts after the first space character.
	std::string message = msg.get_message();
	size_t param_idx = message.find(' ');
	if (param_idx == std::string::npos)
	{
		return "";
	}
	else
	{
		return message.substr(param_idx + 1);
	}
}
