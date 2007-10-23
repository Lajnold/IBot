#include <unistd.h>

#include "DUMIIFinger.h"
#include "IRCBot.h"
#include "IRC_types.h"
#include "IRCSocket.h"
#include "utils.h"

namespace IRC
{
	DUMIIFinger::DUMIIFinger(IRCBot *bot, const char command_char) : CommandHandler(bot, command_char)
	{
		
	}
	
	void DUMIIFinger::handle(const message_list_type &input)
	{
		if(is_command(input, "who"))
		{
			message_list_type list;
			fill_who_list(list);
			
			if(list.size() < 3)
			{
				bot->say("No one visible in DUMII. :(");
				return;
			}
			
			message_list_type out;
			for(message_list_type::const_iterator iter = list.begin(); iter != list.end(); iter++)
				split_string(out, *iter, "\n");
			
			for(message_list_type::const_iterator iter = out.begin(); iter != out.end(); iter++)
				if(!(*iter).empty())
					bot->say(*iter);
		}
	}
	
	void DUMIIFinger::fill_who_list(message_list_type &list)
	{
		IRCSocket socket("\n");
		socket.connect("dum.acc.umu.se", 79);
		
		usleep(200000);
		socket.receive(list);
	}
}
