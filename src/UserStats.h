#ifndef IRC_USERSTATS_H
#define IRC_USERSTATS_H

#include <map>
#include <string>

#include "CommandHandler.h"
#include "IRC_types.h"
#include "IRCBot.h"

namespace IRC
{
	class UserStats : public core::CommandHandler
	{
		struct StatsUser
		{
			StatsUser() {};
			StatsUser(const std::string& nick, int word_count)
			  : nick(nick), word_count(word_count) { };

			std::string nick;
			int word_count;
		};

		typedef std::vector<StatsUser> UserList;
		typedef std::map<std::string, StatsUser> UserMap;
		
		UserMap user_map;
		const std::string filename;
		
		int get_user_word_count(const std::string& nick);
		UserList get_top_list(int count) const;
		
		void update_user_word_count(const std::string& nick, const std::string& message);
		void save_word_count() const;

		struct WordCountComparer
		{
			bool operator()(const StatsUser &lhs, const StatsUser &rhs)
			{
				int lhs_count = lhs.word_count;
				int rhs_count = rhs.word_count;

				if(lhs_count > rhs_count)
					return true;
				if(lhs_count < rhs_count)
					return false;
				else
					return true;
			}
		};
		
	public:
		UserStats(core::IRCBot *bot, const char command_char);
		
		void handle(const core::Message& msg);
	};
}

#endif
