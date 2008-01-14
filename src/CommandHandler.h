#ifndef IRC_COMMANDHANDLER_H
#define IRC_COMMANDHANDLER_H

#include <string>

#include "IRC_types.h"

namespace IRC
{
	namespace core
	{
		class IRCBot;
		
		class CommandHandler
		{
			const char command_char;
			
			
		protected:
			IRCBot *bot;
			
			bool is_msg(const message_list_type &input);
			bool is_channel_msg(const message_list_type &input);
			bool is_command(const message_list_type &input);
			bool is_command(const message_list_type &input, const std::string &command);
			
			std::string get_channel(const message_list_type &input);
			std::string get_command(const message_list_type &input);
			std::string get_message(const message_list_type &input);
			std::string get_user(const message_list_type &input);
			
		public:
			CommandHandler(IRCBot *bot, const char command_character);
			virtual ~CommandHandler() { };
			virtual void handle(const message_list_type &input) = 0;
		};
	}
}

#endif
