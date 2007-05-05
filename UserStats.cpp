#include "UserStats.h"

namespace IRC
{
	void UserStats::increase_word_count(std::string user, unsigned int count)
	{
		users[user].word_count += count;
	}
	
	unsigned int UserStats::get_word_count(std::string user)
	{
		if(users.find(user) == users.end())
			return 0;
		else
			return users[user].word_count;
	}
}
