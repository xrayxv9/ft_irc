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

void sendUsers(Client *cli, Channel *channel)
{
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
	{
		std::ostringstream oss;
		oss << ":ircserv 353 " << cli->getUserName() << " @ " << channel->getName() << " :@" << (*it)->getUserName();
		cli->sendMessage(oss);
	}
	std::ostringstream oss;
	oss << ":ircserv 366 " << cli->getUserName() << " " << channel->getName() << " :End of /NAMES list";
	cli->sendMessage(oss);
}

int Names::execute(const std::string &command, Client *cli) const
{
	std::vector<std::string> channels = split(getArg(command, "NAMES "), ',');
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *channel = cli->getServer().getChannels()[*it];
		if (channel == NULL)
			continue ;
		sendUsers(cli, channel);
	}
	return 1;
}
