#ifndef IRC_USERSTATS_H
#define IRC_USERSTATS_H

#include <map>
#include <string>

#include "IRCCommandHandler.h"
#include "IRC_types.h"

namespace IRC
{
	class IRCBot;
	
	class UserStats : public IRCCommandHandler
	{
		struct Stats
		{
			unsigned int word_count;
			
			Stats() : word_count(0) { };
		};
		
		typedef std::map<std::string, Stats> user_map_type;
		
		user_map_type users;
		const std::string filename;
		
		unsigned int get_word_count_in_msg(const message_list_type &input);
		unsigned int get_word_count(std::string user);
		unsigned int get_user_word_count(const message_list_type &input);
		
		void update_user_word_count(const message_list_type &input);
		void increase_word_count(std::string user, unsigned int count);
		
	public:
		UserStats(IRCBot *bot, const char command_char);
		
		void handle(const message_list_type &input);
	};
}

#endif
