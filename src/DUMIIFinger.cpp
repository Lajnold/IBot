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
using namespace IRC::core;

DUMIIFinger::DUMIIFinger(::IRCBot *bot, const char command_char)
  : CommandHandler(bot, command_char)
{
	
}

void DUMIIFinger::handle(const core::Message& msg)
{
	if(is_command(msg, "who"))
	{
		StringList list = get_who_list();
		
		if(list.size() < 3)
		{
			bot->say("No one visible in DUMII. :(");
			return;
		}
		
		// Remove empty lines.
		list.erase(std::remove(list.begin(), list.end(), ""));

		for(auto it = list.cbegin(); it != list.cend(); ++it)
		{
			bot->say(*it);
		}
	}
}

IRC::StringList DUMIIFinger::get_who_list()
{
	StringList result;

	try
	{
		Socket socket("\n");
		socket.connect("dum.acc.umu.se", 79);

		usleep(250000);
		socket.receive(result);

		if(result.size() < 3)
		{
			usleep(100000);
			socket.receive(result);
		}
	}
	catch(const ConnectionError &e)
	{
		// Do nothing
	}

	return result;
}
