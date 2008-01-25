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
			
			bool is_msg(const packet_t &input);
			bool is_channel_msg(const packet_t &input);
			bool is_command(const packet_t &input);
			bool is_command(const packet_t &input, const std::string &command);
			
			std::string get_channel(const packet_t &input);
			std::string get_command(const packet_t &input);
			std::string get_message(const packet_t &input);
			std::string get_user(const packet_t &input);
			
		public:
			CommandHandler(IRCBot *bot, const char command_character);
			virtual ~CommandHandler() { };
			virtual void handle(const packet_t &input) = 0;
		};
	}
}

#endif
