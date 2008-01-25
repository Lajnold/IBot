#include <stdexcept>
#include <cerrno>
#include <string>
#include <cassert>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "Socket.h"
#include "IRC_types.h"
#include "ConnectionError.h"

using namespace IRC::core;

Socket::Socket(const std::string &line_ending) : line_ending(line_ending)
{
	m_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(m_socket == -1)
	{
		int err = errno;
		throw std::runtime_error(std::strerror(err));
	}
}

Socket::~Socket()
{
	close(m_socket);
}

void Socket::connect(const std::string &address, const unsigned int port)
{
	hostent *he = gethostbyname(address.c_str());
	if(!he)
		throw ConnectionError("Could not find host");

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

void Socket::send(const std::string &data)
{
	std::string to_send(data);

	if(to_send.find("\r\n") == std::string::npos)
		to_send.append("\r\n");

	::send(m_socket, to_send.c_str(), to_send.size(), 0);
}

void Socket::receive(packet_t &out)
{
	char buf[2048];
	int len = recv(m_socket, buf, sizeof(buf), 0);

	if(len == 0)
		throw ConnectionError();

	append_buffer(buf, len);

	while(has_finished_packet())
		out.push_back(get_finished_packet());
}

void Socket::append_buffer(const char *data, const unsigned short len)
{
	buffer += std::string(data, data + len);
}

bool Socket::has_finished_packet()
{
	return buffer.find(line_ending) != std::string::npos;
}

std::string Socket::get_finished_packet()
{
	assert(has_finished_packet());

	std::string packet = buffer.substr(0, buffer.find(line_ending));
	clear_finished_packet();

	return packet;
}

void Socket::clear_finished_packet()
{
	buffer = buffer.substr(buffer.find(line_ending) + 1);
}
