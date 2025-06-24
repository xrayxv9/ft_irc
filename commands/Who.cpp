#include "Who.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include <Client.hpp>
#include <sstream>

Who::Who(): Command("who", "Check who is in the channel")
{
	
}

Who::~Who()
{

}

int Who::execute(const std::string &command, Client *cli) const
{
	std::string channelName = getArg(command, "WHO ");
	Channel *channel = cli->getServer().getChannels()[channelName];
	if (channel == NULL)
	{
		std::cout << "Channel is null" << std::endl;
		return 1;
	}
	std::cout << "Channel is not null" << std::endl;
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end() ; it++)
	{
		Client *user = *it;
		if (user == NULL)
			std::cout << "User is null" << std::endl;
		else
			std::cout << "User is not null" << std::endl;
		std::string toSend;
		std::ostringstream oss;
		oss << user->getFd() << " " << user->generateMask() << " 127.0.0.1 H ircserv :0 " << user->getNickName();
		cli->sendMessage(oss);
	}
	std::ostringstream oss;
	oss << cli->getFd() << " " << cli->generateMask() << " :End of WHO list";
	cli->sendMessage(oss);
	return 1;
}
