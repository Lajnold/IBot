#ifndef IRC_COMMANDHANDLER_H
#define IRC_COMMANDHANDLER_H

#include <string>

#include "IRC_types.h"

namespace IRC
{
	class IRCBot;
	
	class IRCCommandHandler
	{
		
		const char command_char;

	protected:
		IRCBot *bot;
		
		bool is_msg(const message_list_type &input);
		bool is_command(const message_list_type &input);
		bool is_command(const message_list_type &input, const std::string &command);
		
		std::string get_channel(const message_list_type &input);
		std::string get_command(const message_list_type &input);
		std::string get_user(const message_list_type &input);
		
	public:
		IRCCommandHandler(IRCBot *bot, const char command_character);
		virtual ~IRCCommandHandler() { };
		virtual void handle(const message_list_type &input) = 0;
	};
}

#endif
