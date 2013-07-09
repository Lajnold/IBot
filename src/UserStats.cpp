#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>

#include "UserStats.h"
#include "IRC_types.h"
#include "utils.h"
#include "IRCBot.h"

using boost::format;
using boost::algorithm::join;

using namespace IRC;
using namespace IRC::core;

UserStats::UserStats(IRCBot *bot, const char command_char)
  : CommandHandler(bot, command_char),
    filename("stats.txt")
{
	std::ifstream file(filename.c_str());
	if(!file.is_open())
	{
		return;
	}
		
	std::string nick;
	int word_count;
	
	while(file >> nick >> word_count)
	{
		user_map.insert(std::make_pair(nick, StatsUser(nick, word_count)));
	}
}

void UserStats::handle(const Message& msg)
{
	if(is_command(msg, "words"))
	{
		User user = msg.get_user();
		StringList params = get_command_parameters(msg);
		std::string to_send;

		if(params.empty())
		{
			// "Amarth has written 10 words"
			to_send = boost::str(
			      format("%1% has written %2% words")
			        % user.nick
			        % get_user_word_count(user.nick));
		}
		else if(params[0] == "top")
		{
			UserList top_list = get_top_list(5);
			StringList top_list_strings;
			for(auto iter = top_list.cbegin(); iter != top_list.cend(); ++iter)
			{
				// "Amarth (10 words)"
				top_list_strings.push_back(boost::str(
				      format("%1% (%2% words)")
				        % iter->nick
				        % iter->word_count));
			}
				
			to_send = "These are the top writers: " + join(top_list_strings, ", ");
		}
		else
		{
			to_send = params[0] + ": Invalid parameter for words.";
		}

		bot->say(to_send);
	}
	else if (is_channel_message(msg))
	{
		User user = msg.get_user();
		update_user_word_count(user.nick, msg.get_message());
	}
}

int UserStats::get_user_word_count(const std::string& nick)
{
	auto find_it = user_map.find(nick);
	if (find_it == user_map.end())
	{
		return 0;
	}
	else
	{
		return find_it->second.word_count;
	}
}

UserStats::UserList UserStats::get_top_list(int count) const
{
	std::vector<StatsUser> top_list;

	for(auto it = user_map.cbegin(); it != user_map.cend(); it++)
	{
		top_list.push_back(StatsUser(it->second));
	}

	std::sort(top_list.begin(), top_list.end(), WordCountComparer());

	return top_list;
}

void UserStats::update_user_word_count(const std::string& nick, const std::string& message)
{
	if (user_map.find(nick) == user_map.end())
	{
		user_map.insert(std::make_pair(nick, StatsUser(nick, 0)));
	}

	StringList words = utils::split_whitespace<StringList>(message);
	user_map[nick].word_count += words.size();

	save_word_count();
}

void UserStats::save_word_count() const
{
	std::ofstream file(filename.c_str());
	if(!file.is_open())
	{
		return;
	}
		
	for(auto it = user_map.cbegin(); it != user_map.cend(); ++it)
	{
		const StatsUser& user = it->second;
		file << user.nick << " " << user.word_count << std::endl;
	}
}
