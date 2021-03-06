#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#include <string>

#include <sys/socket.h>

#include "IRC_types.h"

namespace IRC
{
	namespace core
	{
		class Socket
		{
			int m_socket;
			std::string buffer;
			std::string line_ending;

			void append_buffer(const char *data, unsigned short len);
			bool has_finished_packet();
			std::string get_finished_packet();
			void clear_finished_packet();

			public:
			Socket(const std::string &line_ending);
			~Socket();

			void connect(const std::string & address, const unsigned int port);
			void send(const std::string &data);
			void receive(StringList &out);
		};
	}
}

#endif
