#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include <Invite.hpp>
#include <sstream>

Invite::Invite(): Command("INVITE", "Invite someone on a channel")
{}

Invite::~Invite()
{}

int Invite::execute(const std::string &command, Client *cli) const
{
	std::string user = getArg(command, this->getName() + ' ');
	std::string channelName = getArg(command, user + ' ');
	
	if (user.empty() || channelName.empty())
	{
		std::ostringstream oss;
		oss << 461 << " " << this->getName() << " :Not enough parameters";
		cli->sendMessage(oss);
		return 0;
	}
	Channel *channel = cli->getServer().getChannels()[channelName];
	if (channel == NULL)
	{
		std::ostringstream oss;
		oss << 403 << " " << channelName << " :No such channel";
		cli->sendMessage(oss);
		return 0;
	}
	if (cli->getChannels()[channelName] == NULL)
	{
		std::ostringstream oss;
		oss << 442 << " " << channelName << " :You're not on that channel";
		cli->sendMessage(oss);
		return 0;
	}
	Client *target = cli->getServer().getClientByString(user);
	if (target == NULL)
		return 0;
	if (target->getChannels()[channelName] != NULL)
	{
		std::ostringstream oss;
		oss << target->getNickName() << " " << channelName << " :is already on channel";
		cli->sendMessage(oss);
		return 0;
	}
	std::ostringstream targetMessage;
	targetMessage << ":" << cli->generateMask() << " INVITE " << user << " :" << channelName;
	cli->sendMessage(targetMessage);
	std::ostringstream senderMessage;
	senderMessage << ":ircserv 341 " << cli->getUserName() << " " << user << " " << channelName;
	target->sendMessage(targetMessage);	
	return 1;
}
