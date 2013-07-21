#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include "BotOptions.h"
#include "ConnectionError.h"
#include "IRC_types.h"
#include "utils.h"

#include "DUMIIFinger.h"
#include "OPify.h"
#include "Say.h"
#include "Time.h"
#include "UserStats.h"

#include "IRCBot.h"

using namespace IRC::core;

using boost::shared_ptr;
using boost::system::error_code;
namespace asio = boost::asio;

IRCBot::IRCBot(const BotOptions &options)
	: running(true), settings(options), socket(io_service)
{
	command_handlers.push_back(shared_ptr<CommandHandler>(new UserStats(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new Time(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new DUMIIFinger(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new Say(this, settings.command_char)));
	command_handlers.push_back(shared_ptr<CommandHandler>(new OPify(this, settings.command_char)));
};

void IRCBot::connect()
{
	asio::ip::tcp::resolver resolver(io_service);
	asio::ip::tcp::resolver::query query(settings.address, std::to_string(settings.port));
	asio::async_connect(socket, resolver.resolve(query),
	                    boost::bind(&IRCBot::handle_connected, this,
	                                asio::placeholders::error,
	                                asio::placeholders::iterator));
}

void IRCBot::handle_connected(const error_code& ec,
                              asio::ip::tcp::resolver::iterator iterator)
{
	if (!ec)
	{
		std::cout << "Connected\n";

		std::string nick = get_nickname();
		std::string channel = get_channel();

		std::string message = 
		      "NICK " + nick + "\r\n"
		    + "USER " + nick + " 0 * :" + nick + "\r\n"
		    + "JOIN " + channel + "\r\n";
		send(message);

		read_next_packet();
	}
	else
	{
		std::cout << "Connect failed\n";
	}
}

std::string IRCBot::get_channel()
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

void IRCBot::queue_message(const std::string& message)
{
	asio::async_write(socket, asio::buffer(message),
	                  boost::bind(&IRCBot::handle_packet_sent, this,
	                              asio::placeholders::error,
	                              asio::placeholders::bytes_transferred));
}

void IRCBot::say(const std::string &message)
{
	std::string channel_message = "PRIVMSG " + get_channel() + " :" + message + "\r\n";
	send(channel_message);
}

void IRCBot::send(const std::string &command)
{
	std::string to_send = command;
	if (!boost::ends_with(to_send, "\r\n"))
	{
		to_send += "\r\n";
	}
	queue_message(to_send);
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

void IRCBot::handle_packet(const std::string &input)
{
	Message msg(input);
	
	if(handle_ping(msg)) { }
	else if(handle_message(msg)) { }
}

void IRCBot::handle_packet_sent(const error_code& e, size_t n_bytes)
{
}

void IRCBot::handle_packet_received(const error_code& err, size_t n_bytes)
{
	if(!err)
	{
		std::istream is(&packet_read_buffer);
		std::vector<char> buf(n_bytes);
		is.read(&buf[0], n_bytes);

		// Use the data except for the trailing \r\n.
		std::string packet(buf.begin(), buf.end() - 2);
		std::cout << "Received packet: " << packet << "\n";
		handle_packet(packet);

		read_next_packet();
	}
	else
	{
		throw std::runtime_error("Receive error");
	}
}

void IRCBot::read_next_packet()
{
	asio::async_read_until(socket, packet_read_buffer, "\r\n",
	                       boost::bind(&IRCBot::handle_packet_received, this, _1, _2));
}

void IRCBot::run()
{	
	connect();
	io_service.run();
}
