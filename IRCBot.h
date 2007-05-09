#ifndef IRCBOT_H
#define IRCBOT_H

#include <vector>
#include <string>

#include "IRCSocket.h"
#include "UserStats.h"

namespace IRC
{
	class IRCBot
	{
		IRC::IRCSocket socket;
		const std::string nickname;
		const std::string channel;
		const char command_char;
		IRC::UserStats stats;
		
		typedef std::vector<std::string> message_list_type;
		
	public:

		IRCBot(const std::string &address, const unsigned int port, const std::string &channel, const char command_char);
		
		void split_string(message_list_type &out, const std::string &input, const std::string &delimiter);
		void split_irc_messages(message_list_type &out, const std::string &input);
		std::string get_command(const message_list_type &input);
		std::string get_user(const message_list_type &input);
		std::string get_time_string();
		std::string make_voice_action(const std::string &action, const message_list_type &input);
		
		bool is_privmsg(const message_list_type &input);
		bool is_privmsg_in_channel(const message_list_type &input);
		bool is_command(const message_list_type &input);
		bool is_ping(const message_list_type &input);
		
		void handle_ping(const message_list_type &input);
		void handle_privmsg(const message_list_type &input);
		
		unsigned int get_word_count_in_privmsg(const message_list_type &input);
		void update_user_word_count(const message_list_type &input);
		unsigned int get_user_word_count(const message_list_type &input);
		
		void parse_irc_message(const std::string &input);
		void parse_data(const std::string &input);
		void run();
	};
}

#endif
