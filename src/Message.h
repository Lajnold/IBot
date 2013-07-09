#ifndef IRC_MESSAGE_H
#define IRC_MESSAGE_H

#include <string>

#include "IRC_types.h"

namespace IRC
{
	namespace core
	{
		enum MessageType
		{
			JOIN,
			PRIVMSG,
			OTHER
		};

		class Message
		{
			const std::string raw_message;
			const StringList tokenized_message;

		public:
			Message(const std::string& raw_message);

			bool is_message() const;

			MessageType get_type() const;
			std::string get_type_string() const;

			StringList get_tokenized_message() const;
			std::string get_raw_message() const;
			std::string get_message() const;
			std::string get_channel() const;
			User get_user() const;
		};
	}
}

#endif
