#include <iostream>
#include <string>

#include "IRCBot.h"
#include "IRC_types.h"
#include "ConnectionError.h"
#include "utils.h"

namespace IRC
{
	IRCBot::IRCBot(const std::string &address, const unsigned int port, const std::string &nickname, const std::string &channel, const char command_char)
	: running(true), message_delimiter(" "), nickname(nickname), channel(channel), stats(this, command_char), time(this, command_char)
	{
		try
		{
			socket.connect(address.c_str(), port);
		}
		catch(const ConnectionError &e)
		{
			std::cout << "Connection failed: " << e.what() << std::endl;
			running = false;
			return;
		}
		
		send("NICK " + nickname);
		send("USER " + nickname + " 0 * :" + nickname);
		send("JOIN " + channel);
	}
	
	void IRCBot::send(std::string message)
	{
		socket.send(message);
	}
	
	void IRCBot::say(const std::string &message)
	{
		std::string channel_message = "PRIVMSG " + channel + " :" + message;
		send(channel_message);
	}

	bool IRCBot::handle_msg(const message_list_type &input)
	{
		time.handle(input);
		stats.handle(input);
		
		return true;
	}

	void IRCBot::parse_IRC_message(const std::string &input)
	{
		message_list_type parameters;
		split_string(parameters, input, message_delimiter);
		
		if(handle_msg(parameters)) { }
	}

	void IRCBot::parse_data(const message_list_type &input)
	{		
		for(message_list_type::const_iterator iter = input.begin(); iter != input.end(); iter++)
		{
			std::cout << *iter << std::endl;
			parse_IRC_message(*iter);
		}
	}

	void IRCBot::run()
	{	
		message_list_type messages;	
		
		while(running)
		{
			messages.clear();
			
			try
			{
				socket.receive(messages);
				parse_data(messages);
			}
			catch(const ConnectionError &e)
			{
				std::cout << "Connection broken" << std::endl;
				running = false;
			}
		}
	}
}
