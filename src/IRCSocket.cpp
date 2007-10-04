#include <stdexcept>
#include <cerrno>
#include <string>
#include <cassert>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "IRCSocket.h"
#include "IRC_types.h"
#include "ConnectionError.h"

namespace IRC
{
	IRCSocket::IRCSocket()
	{
		m_socket = socket(PF_INET, SOCK_STREAM, 0);
		if(m_socket == -1)
		{
			int err = errno;
			throw std::runtime_error(std::strerror(err));
		}
	}

	IRCSocket::~IRCSocket()
	{
		close(m_socket);
	}

	void IRCSocket::connect(const std::string &address, const unsigned int port)
	{
		hostent *he = gethostbyname(address.c_str());
		if(he == NULL)
		{
			throw ConnectionError("Could not find host");
		}
		
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr = *(in_addr *)he->h_addr;
		addr.sin_port = htons(port);
		std::memset(&addr.sin_zero, 0, 8);
		
		if(::connect(m_socket, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
		{
			int err = errno;
			throw ConnectionError(std::strerror(err));
		}
	}

	void IRCSocket::send(const std::string &data)
	{
		std::string to_send(data);
		
		if(to_send.find("\r\n") == std::string::npos)
			to_send.append("\r\n");
			
		::send(m_socket, to_send.c_str(), to_send.size(), 0);
	}

	void IRCSocket::receive(message_list_type &out)
	{
		char buf[2048];
		int len = recv(m_socket, buf, sizeof(buf), 0);
		
		if(len == 0)
			throw ConnectionError();
		
		append_buffer(buf, len);
		
		while(has_finished_packet())
			out.push_back(get_finished_packet());
	}
	
	void IRCSocket::append_buffer(const char *data, const unsigned short len)
	{
		buffer += std::string(data, data + len);
	}
	
	bool IRCSocket::has_finished_packet()
	{
		return buffer.find("\r\n") != std::string::npos;
	}
	
	std::string IRCSocket::get_finished_packet()
	{
		assert(has_finished_packet());
		
		std::string packet = buffer.substr(0, buffer.find("\r\n"));
		clear_finished_packet();
		
		return packet;
	}
	
	void IRCSocket::clear_finished_packet()
	{
		buffer = buffer.substr(buffer.find("\r\n") + 1);
	}
}
