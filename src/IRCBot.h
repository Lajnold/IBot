namespace IRC
{
	namespace core
	{
		class IRCBot;
	}
}

#ifndef IRC_BOT_H
#define IRC_BOT_H

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include "BotOptions.h"
#include "CommandHandler.h"
#include "IRC_types.h"

namespace IRC
{
	namespace core
	{
		class IRCBot
		{
			bool running;

			const BotOptions settings;

			boost::asio::io_service io_service;
			boost::asio::ip::tcp::socket socket;
			boost::asio::streambuf packet_read_buffer;
			//std::vector<unsigned char> packet_read_buffer;
			
			std::vector<boost::shared_ptr<CommandHandler> > command_handlers;

			void connect();
			void queue_message(const std::string& message);
			
			void handle_packet(const std::string &input);
			bool handle_ping(const Message& msg);
			bool handle_message(const Message& msg);

			void handle_connected(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator iterator);
			void handle_packet_sent(const boost::system::error_code& e, size_t n_bytes);
			void handle_packet_received(const boost::system::error_code& e, size_t n_bytes);
			void read_next_packet();
			
		public:

			IRCBot(const BotOptions &options);
			
			const std::string &get_nickname();
			std::string get_channel();
			const std::string &get_owner();
			
			void say(const std::string &input);
			void send(const std::string &command);
			
			void run();
		};
	}
}

#endif
