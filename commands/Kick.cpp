#include "Client.hpp"
#include "Server.hpp"
#include <Kick.hpp>
#include <algorithm>
#include <channel/Channel.hpp>
#include <sstream>

Kick::Kick(): Command("KICK", "kick a user fron a channel")
{
}

Kick::~Kick()
{}

int Kick::execute(const std::string &command, Client *cli) const
{
	(void)command;
	(void)cli;
	
	std::string channelName = getArg(command, this->getName() + ' ');
	std::string user = getArg(command, channelName + ' ');
	if (channelName.empty() || user.empty())
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << this->getName() << " :Not enough parameters";
		cli->sendMessage(oss);
		return 0;
	}
	Channel *channel = cli->getServer().getChannels()[channelName];
	if (channel == NULL)
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << channelName << " :No such channel";
		cli->sendMessage(oss);
		return 0;
	}
	Client *target = cli->getServer().getClientByString(user);
	if (target == NULL)
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << user << " " << channelName << " :They aren't on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	std::vector<Client *>::iterator it = std::find(channel->getClients().begin(), channel->getClients().end(), target);
	if (it == channel->getClients().end())
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << user << " " << channelName << " :They aren't on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	if (cli->getChannels()[channelName] == NULL)
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << channelName << " :You're not on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	if (!cli->isMod(channel))
	{
		std::ostringstream oss;
		oss << ":ircserv 482 " << cli->getUserName() << " " << channelName << " :You're not a channel operator";
		cli->sendMessage(oss);
		return 0;
	}
	std::ostringstream oss;
	oss << ":" << cli->generateMask() << " KICK " << channelName << " " << target->getUserName();
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
		(*it)->sendMessage(oss);
	channel->kick(it);
	return 1;
}
