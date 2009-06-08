#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "IRCBot.h"
#include "BotOptions.h"
#include "IRC_types.h"
#include "ConnectionError.h"
#include "utils.h"

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
		channel = '#' + channel;

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

bool IRCBot::handle_ping(const packet_t &input)
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

bool IRCBot::handle_msg(const packet_t &input)
{
	for(std::vector<shared_ptr<CommandHandler> >::iterator iter = command_handlers.begin();
		iter != command_handlers.end();
		iter++)
		(*iter)->handle(input);
	
	return true;
}

void IRCBot::parse_IRC_message(const std::string &input)
{
	packet_t parameters;
	IRC::utils::split_string(parameters, input, " ");
	
	if(handle_ping(parameters)) { }
	else if(handle_msg(parameters)) { }
}

void IRCBot::parse_data(const packet_t &input)
{		
	for(packet_t::const_iterator iter = input.begin(); iter != input.end(); iter++)
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
	
	packet_t messages;	
	
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
