#include <stdexcept>
#include <string>

#include "utils.h"

#include "Message.h"

IRC::core::Message::Message(const std::string &raw_message)
  : raw_message(raw_message),
    tokenized_message(utils::split_whitespace<StringList>(raw_message))
{
	
}

bool IRC::core::Message::is_message() const
{
	return get_type() == PRIVMSG;
}

IRC::core::MessageType IRC::core::Message::get_type() const
{
	std::string typestr = get_type_string();
	if (typestr == "JOIN")
	{
		return JOIN;
	}
	else if (typestr == "PRIVMSG")
	{
		return PRIVMSG;
	}
	else
	{
		return OTHER;
	}
}

std::string IRC::core::Message::get_type_string() const
{
	if (tokenized_message.size() >= 2)
	{
		return tokenized_message[1];
	}
	else
	{
		return "";
	}
}

IRC::StringList IRC::core::Message::get_tokenized_message() const
{
	return tokenized_message;
}

std::string IRC::core::Message::get_raw_message() const
{
	return raw_message;
}

std::string IRC::core::Message::get_message() const
{
	if(!is_message())
	{
		throw std::logic_error("is not message");
	}

	// The actual message starts with the 4th word.
	// The 4th word in turn starts with a ':', which should be skipped over.
	// The +2 here is for the found ' ' and the ':'.
	auto msg_start_it = utils::find_nth(raw_message, ' ', 3) + 2;
	return std::string(msg_start_it, raw_message.end());
}

std::string IRC::core::Message::get_channel() const
{
	if(!is_message())
	{
		throw std::logic_error("is not message");
	}

	return tokenized_message[2];
}

IRC::User IRC::core::Message::get_user() const
{
	if (tokenized_message.size() < 3)
	{
		throw std::logic_error("message type has no user");
	}
	
	// :<Nick>!<Host>
	const std::string& user_token = tokenized_message[0];
	size_t nick_end = user_token.find('!');
	std::string nick = user_token.substr(1, nick_end - 1);
	std::string host = user_token.substr(nick_end + 1);

	return User(nick, host);
}
