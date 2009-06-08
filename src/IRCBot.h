namespace IRC
{
	namespace core
	{
		class IRCBot;
	}
}

#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "BotOptions.h"
#include "IRC_types.h"
#include "Socket.h"
#include "CommandHandler.h"

namespace IRC
{
	namespace core
	{
		class IRCBot
		{
			bool running;
			
			Socket socket;
			
			const BotOptions settings;
			
			std::vector<boost::shared_ptr<CommandHandler> > command_handlers;

			void connect();
			void send(std::string message);
			
			void parse_IRC_message(const std::string &input);
			void parse_data(const packet_t &input);
			
			bool handle_ping(const packet_t &input);
			bool handle_msg(const packet_t &input);
			
		public:

			IRCBot(const BotOptions &options);
			
			const std::string &get_nickname();
			std::string get_channel();
			const std::string &get_owner();
			
			void say(const std::string &input);
			void raw_command(const std::string &command);
			
			void run();
		};
	}
}

#endif
