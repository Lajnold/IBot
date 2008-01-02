#include <string>

#include "Message.h"

IRC::core::Message::Message(const std::string &msg) : message(msg), cmd_char('!')
{
	
}
