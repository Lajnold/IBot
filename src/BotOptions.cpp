#include <iostream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/program_options.hpp>

#include "BotOptions.h"

namespace
{

void validate_options(IRC::core::BotOptions& options)
{
	if (!boost::starts_with(options.channel, "#"))
	{
		options.channel = "#" + options.channel;
	}
}

} // Anonymous namespace


bool IRC::core::parse_commandline(int argc, char *argv[], BotOptions &options)
{
	namespace po = boost::program_options;

	po::options_description desc;
	desc.add_options()
		("help", "show options")
		("host", po::value<std::string>(&options.address), "the address of the IRC server")
		("port", po::value<unsigned short>(&options.port)->default_value(6667), "the port to connect to")
		("nick", po::value<std::string>(&options.nickname)->default_value("Amon"), "the nickname the bot should use")
		("channel", po::value<std::string>(&options.channel), "the channel the bot should be in")
		("owner", po::value<std::string>(&options.owner)->default_value(""), "the owner of the bot")
		("commandchar", po::value<char>(&options.command_char)->default_value('!'), "the character the bot should react on");

	po::variables_map vm;

	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch(const po::error &e)
	{
		std::cout << e.what() << std::endl << std::endl;
		std::cout << desc << std::endl;

		return false;
	}

	if(vm.count("help") > 0)
	{
		std::cout << desc << std::endl;
		return false;
	}

	validate_options(options);

	return true;
}
