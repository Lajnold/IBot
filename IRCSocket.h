#include <string>

#include <sys/socket.h>

namespace IRC
{
	class IRCSocket
	{
		int m_socket;
		
	public:
		IRCSocket();
		~IRCSocket();
		
		void connect(const std::string & address, const unsigned int port);
		void send(const std::string &data);
		std::string receive();
	};
}
