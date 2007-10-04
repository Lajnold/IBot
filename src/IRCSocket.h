#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#include <string>

#include <sys/socket.h>

#include "IRC_types.h"

namespace IRC
{
	class IRCSocket
	{
		int m_socket;
		std::string buffer;
		
		void append_buffer(const char *data, unsigned short len);
		bool has_finished_packet();
		std::string get_finished_packet();
		void clear_finished_packet();
		
	public:
		IRCSocket();
		~IRCSocket();
		
		void connect(const std::string & address, const unsigned int port);
		void send(const std::string &data);
		void receive(message_list_type &out);
	};
}

#endif
