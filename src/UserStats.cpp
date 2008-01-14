#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "UserStats.h"
#include "IRC_types.h"
#include "utils.h"
#include "IRCBot.h"

using namespace IRC;

UserStats::UserStats(IRC::core::IRCBot *bot, const char command_char) : CommandHandler(bot, command_char), filename("stats.txt")
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

void UserStats::handle(const message_list_type &input)
{
	if(is_channel_msg(input))
		update_user_word_count(input);

	if(is_command(input, "words"))
	{
		std::string to_send;

		message_list_type message;
		split_string(message, get_message(input), " ");

		if(message.size() == 1)
			to_send = get_user(input) + " has written " +
			boost::lexical_cast<std::string>(get_user_word_count(input)) + 
			" words.";
		else if(message[1] == "top")
		{
			message_list_type list;
			fill_top_list(list, 5);

			to_send = "These are the top writers: ";

			for(message_list_type::iterator iter = list.begin();
				iter != list.end();
				iter++)
			{
				to_send += *iter + " (" + 
				boost::lexical_cast<std::string>(users[*iter].word_count) +
				" words)";

				if(iter + 1 != list.end())
					to_send += ", ";
			}
				
		}
		else
			to_send = message[1] + ": Invalid parameter for words.";

		bot->say(to_send);
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
	return users[user].word_count;
}

unsigned int UserStats::get_word_count_in_msg(const message_list_type &input)
{
	assert(is_msg(input));
	
	unsigned int count = 0;
	std::string message = input[3];
	
	for(message_list_type::const_iterator iter = input.begin() + 4; iter != input.end(); iter++)
		message += " " + *iter;
	
	message_list_type splitted_message;
	split_string(splitted_message, message, " ");
	
	for(message_list_type::const_iterator iter = splitted_message.begin(); iter != splitted_message.end(); iter++)
	{
		if((*iter).find_first_not_of(" \t") != std::string::npos)
			count++;
	}
	
	return count;
}

void UserStats::update_user_word_count(const message_list_type &input)
{
	assert(is_msg(input));
	
	std::string user = get_user(input);
	unsigned int count = get_word_count_in_msg(input);
	
	increase_word_count(user, count);
}

unsigned int UserStats::get_user_word_count(const message_list_type &input)
{
	std::string user = get_user(input);
	return get_word_count(user);
}

void UserStats::fill_top_list(message_list_type &out, size_t count)
{
	std::vector<User> list;

	for(user_map_type::const_iterator iter = users.begin();
		iter != users.end();
		iter++)
		list.push_back(User((*iter).first, (*iter).second));

	std::sort(list.begin(), list.end(), WordCountSort());

	for(size_t i = 0; i < count && i < users.size(); i++)
		out.push_back(list[i].name);
}
