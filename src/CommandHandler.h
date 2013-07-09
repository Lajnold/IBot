#ifndef IRC_COMMANDHANDLER_H
#define IRC_COMMANDHANDLER_H

#include <string>

#include "IRC_types.h"
#include "Message.h"

namespace IRC
{
	namespace core
	{
		// Forward declaration.
		class IRCBot;

		class CommandHandler
		{
			const char command_char;
			
		protected:
			IRCBot *bot;
			
			bool is_channel_message(const Message& msg);
			bool is_command(const Message& msg);
			bool is_command(const Message& msg, const std::string &command);
			
			std::string get_command(const Message& msg);
			StringList get_command_parameters(const Message& msg);
			std::string get_command_parameter_string(const Message& msg);
			
		public:
			CommandHandler(IRCBot *bot, const char command_character);
			virtual ~CommandHandler() { };
			virtual void handle(const Message& msg) = 0;
		};
	}
}

#endif
