#include "UserStats.h"

#include <fstream>
#include <string>

namespace IRC
{
	UserStats::UserStats() : filename("stats.txt")
	{
		std::ifstream file(filename.c_str());
		if(!file.is_open())
			return;
			
		std::string user;
		unsigned int word_count;
		
		while(file >> user >> word_count)
		{
			users[user].word_count = word_count;
		}
	}
	
	void UserStats::increase_word_count(std::string user, unsigned int count)
	{
		users[user].word_count += count;
		
		std::ofstream file(filename.c_str());
		if(!file.is_open())
			return;
			
		for(user_map_type::const_iterator iter = users.begin(); iter != users.end(); iter++)
		{
			file << (*iter).first << " " << (*iter).second.word_count << std::endl;
		}
	}
	
	unsigned int UserStats::get_word_count(std::string user)
	{
		if(users.find(user) == users.end())
			return 0;
		else
			return users[user].word_count;
	}
}
