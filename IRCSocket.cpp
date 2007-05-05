#include <stdexcept>
#include <cerrno>
#include <string>

#include <sys/socket.h>
#include <netdb.h>

#include "IRCSocket.h"

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
			throw std::runtime_error("Could not find host");
		}
		
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr = *(in_addr *)he->h_addr;
		addr.sin_port = htons(port);
		std::memset(&addr.sin_zero, 0, 8);
		
		if(::connect(m_socket, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
		{
			int err = errno;
			throw std::runtime_error(std::strerror(err));
		}
	}

	void IRCSocket::send(const std::string &data)
	{
		std::string to_send(data);
		if(to_send.find("\r\n") == std::string::npos)
			to_send.append("\r\n");
			
		::send(m_socket, to_send.c_str(), to_send.size(), 0);
	}

	std::string IRCSocket::receive()
	{
		char buf[1024];
		int len = recv(m_socket, buf, sizeof(buf) - 1, 0);
		return std::string(buf, buf + len);
	}
}
