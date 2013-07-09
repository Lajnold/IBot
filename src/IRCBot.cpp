#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "IRC_types.h"
#include "BotOptions.h"
#include "ConnectionError.h"
#include "utils.h"

#include "IRCBot.h"

#include "Time.h"
#include "UserStats.h"
#include "DUMIIFinger.h"
#include "Say.h"
#include "OPify.h"

using namespace IRC::core;
using boost::shared_ptr;

IRCBot::IRCBot(const BotOptions &options)
	: running(true), socket("\r\n"), settings(options)
{
	command_handlers.push_back(shared_ptr<CommandHandler>(new UserStats(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new Time(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new DUMIIFinger(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new Say(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new OPify(this, settings.command_char)));
};

void IRCBot::connect()
{
	socket.connect(settings.address, settings.port);
	
	send("NICK " + settings.nickname);
	send("USER " + settings.nickname + " 0 * :" + settings.nickname);

	send("JOIN " + get_channel());
}

std::string IRCBot::get_channel()
{
	std::string channel = settings.channel;
	if(channel.size() > 0 && channel[0] != '#')
	{
		channel = '#' + channel;
	}

	return channel;
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
	std::string channel_message = "PRIVMSG " + get_channel() + " :" + message;
	send(channel_message);
}

void IRCBot::raw_command(const std::string &command)
{
	send(command);
}

bool IRCBot::handle_ping(const Message& msg)
{
	// Input format: "PING :<server1> [<server2>]"
	// Return format: "PONG <server1>"
	// Note that this code currently only handles <server1>.

	StringList tokenized_message = msg.get_tokenized_message();
	if(tokenized_message.empty() || tokenized_message[0] != "PING")
	{
		return false;
	}

	std::string return_message = "PONG ";
	if(tokenized_message.size() >= 2)
	{
		return_message += tokenized_message[1].substr(1);
	}
	else
	{
		return_message += "dummy.server";
	}

	send(return_message);
	
	return true;
}

bool IRCBot::handle_message(const Message& msg)
{
	for(auto it = command_handlers.begin(); it != command_handlers.end(); it++)
	{
		(*it)->handle(msg);
	}

	return true;
}

void IRCBot::parse_IRC_message(const std::string &input)
{
	Message msg(input);
	
	if(handle_ping(msg)) { }
	else if(handle_message(msg)) { }
}

void IRCBot::parse_data(const StringList &input)
{
	for(auto iter = input.cbegin(); iter != input.cend(); ++iter)
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
	
	StringList messages;	
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
