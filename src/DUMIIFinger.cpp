#include <ctime>
#include <algorithm>

#include <unistd.h>

#include "DUMIIFinger.h"
#include "IRCBot.h"
#include "IRC_types.h"
#include "Socket.h"
#include "ConnectionError.h"
#include "utils.h"

using namespace IRC;

DUMIIFinger::DUMIIFinger(IRC::core::IRCBot *bot, const char command_char) : CommandHandler(bot, command_char)
{
	
}

void DUMIIFinger::handle(const packet_t &input)
{
	if(is_command(input, "who"))
	{
		packet_t list;
		fill_who_list(list);
		
		if(list.size() < 3)
		{
			bot->say("No one visible in DUMII. :(");
			return;
		}
		
		list.erase(std::remove(list.begin(), list.end(), ""));

		for(packet_t::const_iterator iter = list.begin();
			iter != list.end();
			iter++)
			bot->say(*iter);
	}
}

void DUMIIFinger::fill_who_list(packet_t &list)
{
	try
	{
		Socket socket("\n");
		socket.connect("dum.acc.umu.se", 79);

		usleep(250000);
		socket.receive(list);

		if(list.size() < 3)
		{
			usleep(100000);
			socket.receive(list);
		}
	}
	catch(const ConnectionError &e)
	{

	}
}
