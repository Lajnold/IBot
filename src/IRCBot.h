#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <string>
#include <vector>

#include "IRC_types.h"
#include "Socket.h"
#include "CommandHandler.h"

namespace IRC
{
	class IRCBot
	{
		bool running;
		
		Socket socket;
		
		const std::string message_delimiter;
		
		const std::string address;
		const unsigned short port;
		
		const std::string nickname;
		const std::string channel;
		
		std::vector<CommandHandler *> command_handlers;

		void connect();
		void send(std::string message);
		
		void parse_IRC_message(const std::string &input);
		void parse_data(const message_list_type &input);
		
		bool handle_ping(const message_list_type &input);
		bool handle_msg(const message_list_type &input);
		
	public:

		IRCBot(const std::string &address, const unsigned int port, const std::string &nickname, const std::string &channel, const char command_char);
		~IRCBot();
		
		const std::string &get_nickname();
		const std::string &get_channel();
		
		void say(const std::string &input);
		
		void run();
	};
}

#endif
