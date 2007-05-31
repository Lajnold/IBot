#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <string>

#include "IRC_types.h"
#include "IRCSocket.h"
#include "Time.h"
#include "UserStats.h"

namespace IRC
{
	class IRCBot
	{
		bool running;
		
		IRCSocket socket;
		const std::string message_delimiter;
		const std::string nickname;
		const std::string channel;
		
		UserStats stats;
		Time time;
		
		void send(std::string message);
		
		void parse_IRC_message(const std::string &input);
		void parse_data(const message_list_type &input);
		
		bool handle_msg(const message_list_type &input);
		
	public:

		IRCBot(const std::string &address, const unsigned int port, const std::string &nickname, const std::string &channel, const char command_char);
		
		const std::string &get_nickname();
		const std::string &get_channel();
		
		void say(const std::string &input);
		
		void run();
	};
}

#endif
