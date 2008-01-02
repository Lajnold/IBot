#include <iostream>
#include <string>
#include <vector>

#include "IRCBot.h"
#include "BotOptions.h"
#include "IRC_types.h"
#include "ConnectionError.h"
#include "utils.h"

#include "Time.h"
#include "UserStats.h"
#include "DUMIIFinger.h"
#include "Say.h"

namespace IRC
{
	IRCBot::IRCBot(const BotOptions &options)
		: running(true), socket("\r\n"), settings(options)
	{
		command_handlers.push_back(new UserStats(this, settings.command_char));
		command_handlers.push_back(new Time(this, settings.command_char));
		command_handlers.push_back(new DUMIIFinger(this, settings.command_char));
		command_handlers.push_back(new Say(this, settings.command_char));
	};

	IRCBot::~IRCBot()
	{
		for(std::vector<CommandHandler *>::iterator iter = command_handlers.begin();
			iter != command_handlers.end();)
		{
			delete *iter;
			iter = command_handlers.erase(iter);
		}
	}
	
	void IRCBot::connect()
	{
		socket.connect(settings.address.c_str(), settings.port);
		
		send("NICK " + settings.nickname);
		send("USER " + settings.nickname + " 0 * :" + settings.nickname);
		send("JOIN " + settings.channel);
	}
	
	const std::string &IRCBot::get_channel()
	{
		return settings.channel;
	}
	
	const std::string &IRCBot::get_nickname()
	{
		return settings.nickname;
	}

	const std::string &IRCBot::get_owner()
	{
		return settings.owner;
	}
	
	void IRCBot::send(std::string message)
	{
		socket.send(message);
	}
	
	void IRCBot::say(const std::string &message)
	{
		std::string channel_message = "PRIVMSG " + settings.channel + " :" + message;
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
		for(std::vector<CommandHandler *>::iterator iter = command_handlers.begin();
			iter != command_handlers.end();
			iter++)
			(*iter)->handle(input);
		
		return true;
	}

	void IRCBot::parse_IRC_message(const std::string &input)
	{
		message_list_type parameters;
		split_string(parameters, input, " ");
		
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
