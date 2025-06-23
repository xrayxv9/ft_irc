#include "Client.hpp"
#include "Server.hpp"
#include <PrivMSG.hpp>
#include <sstream>

PrivMSG::PrivMSG(): Command("privmsg", "private messages")
{
}

PrivMSG::~PrivMSG()
{
}

int PrivMSG::execute(const std::string &command, Client *cli) const
{
	std::cout << "----------" << cli->getFd() << "----------" << std::endl << command << std::endl << "---------------------" << std::endl;
	Channel *channel = cli->getServer().getChannels()[getArg(command, "PRIVMSG ")];
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
