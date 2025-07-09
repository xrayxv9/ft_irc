#include "Client.hpp"
#include "Server.hpp"
#include <PrivMSG.hpp>
#include <map>
#include <sstream>

PrivMSG::PrivMSG(): Command("PRIVMSG", "Send messages in dm or in channels")
{
}

PrivMSG::~PrivMSG()
{
}

int PrivMSG::execute(const std::string &command, Client *cli) const
{
	std::map<std::string, Channel *>::iterator it = cli->getServer().getChannels().find(getArg(command, "PRIVMSG "));
	if (it == cli->getServer().getChannels().end())
	{
		Client *target = cli->getServer().getClientByString(getArg(command, "PRIVMSG "));
		if (target == NULL)
		{
			std::ostringstream oss;
			oss << getArg(command, "PRIVMSG ") << " :No such nick/channel";
			cli->sendMessage(oss);
			return 0;
		}
		std::ostringstream oss;
		oss << ":" << cli->generateMask() << " PRIVMSG " << target->getUserName() << " :" << getArg(command, " :", true),
		target->sendMessage(oss);
		return 1;
	}
	Channel *channel = cli->getServer().getChannels()[getArg(command, "PRIVMSG ")];
	if (!cli->isInChannel(channel))
	{
		std::ostringstream oss;
		oss << cli->getUserName() << " " << channel->getName() << " :Cannot send to channel";
		cli->sendMessage(oss);
		return 0;
	}
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
	{
		Client *user = *it;
		std::ostringstream oss;
		if (cli == user)
			continue ;
		oss << ":" << cli->getNickName() << " PRIVMSG " << getArg(command, "PRIVMSG ") << " :" << getArg(command, " :", true); 
		user->sendMessage(oss);
	}
	return 1;
}
