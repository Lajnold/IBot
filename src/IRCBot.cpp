#include <iostream>
#include <string>

#include "IRCBot.h"
#include "IRC_types.h"
#include "ConnectionError.h"
#include "utils.h"

namespace IRC
{
	IRCBot::IRCBot(const std::string &address, const unsigned int port, const std::string &nickname, const std::string &channel, const char command_char)
	: running(true), socket("\r\n"), message_delimiter(" "), address(address), port(port), nickname(nickname), channel(channel), stats(this, command_char), 
		time(this, command_char), finger(this, command_char) 
	{
		
	}
	
	void IRCBot::connect()
	{
		socket.connect(address.c_str(), port);
		
		send("NICK " + nickname);
		send("USER " + nickname + " 0 * :" + nickname);
		send("JOIN " + channel);
	}
	
	const std::string &IRCBot::get_channel()
	{
		return channel;
	}
	
	const std::string &IRCBot::get_nickname()
	{
		return nickname;
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
	
	bool IRCBot::handle_ping(const message_list_type &input)
	{
		if(input.size() < 1 || input[0] != "\nPING")
			return false;
		
		std::string message = "PONG ";
		if(input.size() >= 2)
			message += input[1].substr(1);
		else
			message += "dummy.server";
		
		send(message);
		
		return true;
	}

	bool IRCBot::handle_msg(const message_list_type &input)
	{
		time.handle(input);
		stats.handle(input);
		finger.handle(input);
		
		return true;
	}

	void IRCBot::parse_IRC_message(const std::string &input)
	{
		message_list_type parameters;
		split_string(parameters, input, message_delimiter);
		
		if(handle_ping(parameters)) { }
		else if(handle_msg(parameters)) { }
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
		try
		{
			connect();
		}
		catch(const ConnectionError &e)
		{
			std::cout << "Connection failed: " << e.what() << std::endl;
			running = false;
		}
		
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
