#include "Client.hpp"
#include "Server.hpp"
#include <Kick.hpp>
#include <algorithm>
#include <channel/Channel.hpp>
#include <sstream>
#include <string>

Kick::Kick(): Command("KICK", "kick a user fron a channel")
{
}

Kick::~Kick()
{}

int Kick::execute(const std::string &command, Client *cli) const
{
	std::string channelName = getArg(command, this->getName() + ' ');
	std::string user = getArg(command, channelName + ' ');
	if (channelName.empty() || user.empty())
	{
		std::ostringstream oss;
		oss << ":ircserv 461 " << cli->getUserName() << " " << this->getName() << " :Not enough parameters";
		cli->sendMessage(oss);
		return 0;
	}
	std::map<std::string, Channel *>::iterator channel = cli->getServer().getChannels().find(channelName);
	if (channel == cli->getServer().getChannels().end())
	{
		std::ostringstream oss;
		oss << ":ircserv 403 " << cli->getUserName() << " " << channelName << " :No such Channel";
		cli->sendMessage(oss);
		return 0;
	}
	Client *target = cli->getServer().getClientByString(user);
	if (target == NULL)
	{
		std::ostringstream oss;
		oss << ": 441 " << cli->getNickName() << " " << user << " " << channelName << " :They aren't on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	std::vector<Client *>::iterator it = std::find(channel->second->getClients().begin(), channel->second->getClients().end(), target);
	if (it == channel->second->getClients().end())
	{
		std::ostringstream oss;
		oss << ": 441 " << cli->getNickName() << " " << user << " " << channelName << " :They aren't on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	if (cli->getChannels()[channelName] == NULL)
	{
		std::ostringstream oss;
		oss << ": 442 " << cli->getNickName() << " " << channelName << " :You're not on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	if (!cli->isMod(channel->second))
	{
		std::ostringstream oss;
		oss << ": 482 " << cli->getUserName() << " " << channelName << " :You're not a channel operator";
		cli->sendMessage(oss);
		return 0;
	}
	std::ostringstream oss;
	oss << ":" << cli->generateMask() << " KICK " << channelName << " " << target->getNickName() << " :";
	std::string reason = getArg(command, " :", true);
	if (!reason.empty())
		oss << reason;
	else
		oss << target->getNickName();
	for (std::vector<Client *>::iterator it = channel->second->getClients().begin(); it != channel->second->getClients().end(); it++)
		(*it)->sendMessage(oss);
	std::cout<< "Before kick " << channel->second->getName() << " " << cli->getChannels().size() << std::endl;
	channel->second->kick(*it);
	std::cout<< "After kick " << cli->getChannels().size() << std::endl;
	return 1;
}
