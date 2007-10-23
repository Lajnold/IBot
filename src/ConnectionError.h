#ifndef CONNECTIONERROR_H
#define CONNECTIONERROR_H

#include <stdexcept>

namespace IRC
{
	class ConnectionError : public std::runtime_error
	{
		
	public:
		ConnectionError(const char *msg = "") : runtime_error(msg) { };
	};
}

#endif
