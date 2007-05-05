#include "IRCSocket.h"

#include <iostream>
#include <vector>
#include <string>

class IRCBot
{
	IRCSocket socket;
	const std::string nickname;
	const std::string channel;
	
	typedef std::vector<std::string> message_list_type;
	
public:

	IRCBot(const std::string &address, const unsigned int port, const std::string &channel) : nickname("Amon"), channel(channel)
	{
		socket.connect(address.c_str(), port);
		socket.send("NICK " + nickname);
		socket.send("USER Amon 0 * :Amon");
		socket.send("JOIN " + channel);
	}

	void split_string(message_list_type &out, const std::string &input, const std::string &delimiter)
	{
		std::string::size_type first = 0, second = input.find(delimiter);
		
		out.push_back(input.substr(first, second));
		
		while(second != std::string::npos)
		{
			first = second + 1;
			second = input.find(delimiter, first);
			out.push_back(input.substr(first, second - first));
		}
	}

	void split_irc_messages(message_list_type &out, const std::string &input)
	{
		split_string(out, input, "\r\n");
	}
	
	bool is_privmsg_in_channel(const message_list_type &input)
	{
		return input.size() >= 4 && input[1] == "PRIVMSG" && input[2] == channel;
	}
	
	bool is_command(const message_list_type &input)
	{
		return input.size() >= 4 && input[3][1] == '!';
	}
	
	std::string get_command(const message_list_type &input)
	{
		return input[3].substr(2);
	}
	
	std::string make_action(std::string action, const message_list_type &input)
	{
		std::string message = "PRIVMSG " + channel + " :\1ACTION " + action;
		
		for(message_list_type::const_iterator iter = input.begin() + 4; iter != input.end(); iter++)
			message += " " + *iter;
			
		return message;
	}

	void parse_irc_message(const std::string &input)
	{
		message_list_type parameters;
		split_string(parameters, input, " ");
		
		if(is_privmsg_in_channel(parameters))
		{
			if(is_command(parameters))
			{
				std::string command = get_command(parameters);
				
				if(command == "yell")
				{			
					std::string action = make_action("yells", parameters);
					socket.send(action);
				}
				
				else if(command == "whisper")
				{
					std::string action = make_action("whispers", parameters);
					socket.send(action);
				}
			}
		}
		
		else if(parameters.size() >= 1 && parameters[0] == "PING")
		{
			socket.send("PONG irc.dot.net");
		}
	}

	void parse_irc_data(const std::string &input)
	{
		message_list_type splitted_messages;
		split_irc_messages(splitted_messages, input);
		
		for(message_list_type::const_iterator iter = splitted_messages.begin(); iter != splitted_messages.end(); iter++)
		{
			std::cout << *iter;
			parse_irc_message(*iter);
		}
	}
	
	void run()
	{		
		while(true)
		{
			parse_irc_data(socket.receive());
		}
	}
};


int main()
{
	IRCBot bot("127.0.0.1", 9997, "#qwer");
	bot.run();
}
