#ifndef IRC_COMMAND_H
#define IRC_COMMAND_H

#include <vector>
#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"

namespace IRC
{
	class OPify : public core::CommandHandler
	{
		std::vector<std::string> auto_op;

		bool should_auto_op(const std::string& nick);
		void make_op(const std::string& nick);

	public:
		OPify(core::IRCBot *bot, const char command_char);

		void handle(const core::Message& msg);
	};
}

#endif 
