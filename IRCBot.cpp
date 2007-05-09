#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include <cstring>

#include <boost/lexical_cast.hpp>

#include "IRCBot.h"

namespace IRC
{
	IRCBot::IRCBot(const std::string &address, const unsigned int port, const std::string &channel, const char command_char)
	: nickname("Amon"), channel(channel), command_char(command_char)
	{
		socket.connect(address.c_str(), port);
		socket.send("NICK " + nickname);
		socket.send("USER Amon 0 * :Amon");
		socket.send("JOIN " + channel);
	}
	
	void IRCBot::send(std::string message)
	{
		socket.send(message);
	}

	void IRCBot::split_string(message_list_type &out, const std::string &input, const std::string &delimiter)
	{
		std::string::size_type first = 0, second = input.find(delimiter);
		
		out.push_back(input.substr(first, second));
		
		while(second != std::string::npos)
		{
			first = second + 1;
			second = input.find(delimiter, first);
			out.push_back(input.substr(first, second - first));
		}
	}

	void IRCBot::split_irc_messages(message_list_type &out, const std::string &input)
	{
		split_string(out, input, "\r\n");
	}

	bool IRCBot::is_privmsg(const message_list_type &input)
	{
		return input.size() >= 4 && input[1] == "PRIVMSG";
	}

	bool IRCBot::is_privmsg_in_channel(const message_list_type &input)
	{
		return is_privmsg(input) && get_channel(input) == channel;
	}

	bool IRCBot::is_command(const message_list_type &input)
	{
		return is_privmsg(input) && input[3][1] == command_char;
	}
	
	std::string IRCBot::get_channel(const message_list_type &input)
	{
		assert(is_privmsg(input));
		
		return input[2];
	}

	std::string IRCBot::get_command(const message_list_type &input)
	{
		assert(is_command(input));
		
		return input[3].substr(2);
	}

	std::string IRCBot::make_voice_action(const std::string &action, const message_list_type &input)
	{
		std::string message = "PRIVMSG " + channel + " :\1ACTION " + action;
		
		if(input.size() >= 5)
		{
			message += " '" + input[4];
			
			for(message_list_type::const_iterator iter = input.begin() + 5; iter != input.end(); iter++)
				message += " " + *iter;
				
			message += "'";
		}
			
		return message;
	}

	bool IRCBot::is_ping(const message_list_type &input)
	{
		return input.size() >= 1 && input[0] == "PING";
	}

	std::string IRCBot::get_time_string()
	{
		char str[256];
		std::time_t t = std::time(NULL);		
		std::tm *tm = std::localtime(&t);
		std::strftime(str, sizeof(str), "Wohoo! It's %A today! %B %d, in the year of %Y, actually. And the time is %H:%M:%S. But seriously, why don't you just look at the computer clock?", tm);
		return std::string(str, str + std::strlen(str));
	}

	std::string IRCBot::get_user(const message_list_type &input)
	{
		assert(is_privmsg(input));
		
		size_t pos = input[0].find("!");
		return input[0].substr(1, pos - 1);
	}

	unsigned int IRCBot::get_word_count_in_privmsg(const message_list_type &input)
	{
		assert(is_privmsg(input));
		
		unsigned int count = 0;
		
		for(message_list_type::const_iterator iter = input.begin() + 3; iter != input.end(); iter++)
		{
			if((*iter).find_first_not_of(" \t") != std::string::npos)
				count++;
		}
		
		return count;
	}

	void IRCBot::update_user_word_count(const message_list_type &input)
	{
		assert(is_privmsg(input));
		
		std::string user = get_user(input);
		unsigned int count = get_word_count_in_privmsg(input);
		stats.increase_word_count(user, count);
	}

	unsigned int IRCBot::get_user_word_count(const message_list_type &input)
	{
		std::string user = get_user(input);
		return stats.get_word_count(user);
	}
	
	bool IRCBot::handle_ping(const message_list_type &input)
	{
		if(!is_ping(input))
			return false;
			
		std::string data = "PONG ";
		
		if(input.size() >= 2)
			data += input[1].substr(1);
		else
			data += "dummy.server";
		
		send(data);
		
		return true;
	}
	
	bool IRCBot::handle_yell(const message_list_type &input)
	{
		if(!is_command(input) || get_command(input) != "yell")
			return false;
			
		std::string message = make_voice_action("yells", input);
		send(message);
		
		return true;
	}
	
	bool IRCBot::handle_whisper(const message_list_type &input)
	{
		if(!is_command(input) || get_command(input) != "whisper")
			return false;
			
		std::string message = make_voice_action("whispers", input);
		send(message);
		
		return true;
	}
	
	bool IRCBot::handle_time(const message_list_type &input)
	{
		if(!is_command(input) || get_command(input) != "time")
			return false;
			
		std::string message = "PRIVMSG " + channel + " :" + get_time_string();
		send(message);
		
		return true;
	}
	
	bool IRCBot::handle_words(const message_list_type &input)
	{
		if(!is_command(input) || get_command(input) != "words")
			return false;
			
		std::string message = "PRIVMSG " + channel + " :" + get_user(input) + " has written " + boost::lexical_cast<std::string>(get_user_word_count(input)) + " words.";
		send(message);
		
		return true;
	}

	bool IRCBot::handle_privmsg(const message_list_type &input)
	{
		if(!is_privmsg(input))
			return false;
		
		update_user_word_count(input);
		
		if(handle_yell(input)) { }
		else if(handle_whisper(input)) { }
		else if(handle_time(input)) { }
		else if(handle_words(input)) { }
		
		return true;
	}

	void IRCBot::parse_irc_message(const std::string &input)
	{
		message_list_type parameters;
		split_string(parameters, input, " ");
		
		if(handle_privmsg(parameters)) { }
		else if(handle_ping(parameters)) { }
	}

	void IRCBot::parse_data(const std::string &input)
	{
		message_list_type splitted_messages;
		split_irc_messages(splitted_messages, input);
		
		for(message_list_type::const_iterator iter = splitted_messages.begin(); iter != splitted_messages.end(); iter++)
		{
			std::cout << *iter;
			parse_irc_message(*iter);
		}
	}

	void IRCBot::run()
	{		
		while(true)
		{
			parse_data(socket.receive());
		}
	}
}
