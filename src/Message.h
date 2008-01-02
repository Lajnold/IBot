#ifndef IRC_MESSAGE_H
#define IRC_MESSAGE_H

#include <string>

namespace IRC
{
	namespace core
	{
		class Message
		{
			const std::string message;
			const char cmd_char;

			enum message_type
			{
				MESSAGE_TYPE_MSG = 0x1,
				MESSAGE_TYPE_CHANNEL_MSG = 0x2,
				MESSAGE_TYPE_CMD = 0x4
			};

		public:
			Message(const std::string &msg);
		};
	}
}

#endif
