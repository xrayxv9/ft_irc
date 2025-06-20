#include "Who.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include <Client.hpp>

Who::Who(): Command("who", "Check who is in the channel")
{
	
}

Who::~Who()
{

}

int Who::execute(const std::string &command, Client &cli) const
{
	return 1;
	std::string channelName = getArg(command, "WHO ");
	Channel *channel = cli.getServer().getChannels()[channelName];
	// std::cout << "--------------------------------" << std::endl << "'" << command << "'" << std::endl << "--------------------------------" << std::endl;
	// std::cout << "--------------------------------" << std::endl << "'" << channelName << "'" << std::endl << "--------------------------------" << std::endl;
	std::cout << "Channel name is: '" << channelName << "' and len of all channels is: " << cli.getServer().getChannels().size()  << std::endl;
	for (std::map<std::string, Channel *>::iterator it = cli.getServer().getChannels().begin() ; it != cli.getServer().getChannels().end(); it ++)
		std::cout << "'" << it->second->getName() << "'" << std::endl;
	if (channel == NULL)
		return 1;
	std::vector<Client> clients = channel->getClients();
	for (std::vector<Client>::iterator it = clients.begin(); it != channel->getClients().end() ; it++)
	{
		cli.sendMessage(cli.getName());	
	}

	return 1;
}
