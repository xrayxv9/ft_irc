#include "Names.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include <sstream>
#include <vector>
#include <main.hpp>

Names::Names(): Command("NAMES", "See who is here")
{
}

Names::~Names()
{}

//TODO: Permissions on which channel user can see

int Names::execute(const std::string &command, Client *cli) const
{
	std::vector<std::string> channels = split(getArg(command, "NAMES "), ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = cli->getServer().getChannels()[*it];
		if (channel == NULL)
			continue ;
		cli->sendUsersList(channel);
	}
	return 1;
}
