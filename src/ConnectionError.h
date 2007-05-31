#ifndef CONNECTIONERROR_H
#define CONNECTIONERROR_H

#include <stdexcept>
#include <string>

namespace IRC
{
	class ConnectionError : public std::runtime_error
	{
		
	public:
		ConnectionError(const std::string &msg = "") : runtime_error(msg) { };
	};
}

#endif
