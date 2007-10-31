#ifndef IRC_USERSTATS_H
#define IRC_USERSTATS_H

#include <map>
#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class UserStats : public CommandHandler
	{
		struct Stats
		{
			unsigned int word_count;
			
			Stats() : word_count(0) { };
		};

		struct User
		{
			User(std::string name, Stats stats) : name(name), stats(stats) { };
			std::string name;
			Stats stats;
		};
		
		typedef std::map<std::string, Stats> user_map_type;
		
		user_map_type users;
		const std::string filename;
		
		unsigned int get_word_count_in_msg(const message_list_type &input);
		unsigned int get_word_count(std::string user);
		unsigned int get_user_word_count(const message_list_type &input);
		void fill_top_list(message_list_type &out, int count);
		
		void update_user_word_count(const message_list_type &input);
		void increase_word_count(std::string user, unsigned int count);

		struct WordCountSort
		{
			bool operator()(const User &lhs, const User &rhs)
			{
				int lhs_count = lhs.stats.word_count;
				int rhs_count = rhs.stats.word_count;

				if(lhs_count > rhs_count)
					return true;
				if(lhs_count < rhs_count)
					return false;
				else
					return true;
			}
		};
		
	public:
		UserStats(IRCBot *bot, const char command_char);
		
		void handle(const message_list_type &input);
	};
}

#endif
