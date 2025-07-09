#include "Join.hpp"
#include "../client/Client.hpp"
#include "Server.hpp"
#include <cstddef>
#include <sstream>
#include <vector>
#include "../main.hpp"

Join::Join(): Command("JOIN", "Join a channel")
{

}

void joinChannel(Client *cli, const std::string &name, const std::string &pass)
{
	if (name[0] != '#')
	{
		std::ostringstream oss;
		oss << ":ircserv " << 403 << " " << cli->getUserName() << " " << name << " :No such channel";
		cli->sendMessage(oss);
		return ;
	}
	Channel *channel = cli->joinChannel(name, pass);
	if (channel == NULL)
	{
		return ;
	}
	channel->getClients().push_back(cli);
}



//TODO: TOO_MANY_CHANNELS, INVITE_ONLY

int Join::execute(const std::string &command, Client *cli) const
{
	//Parsing

	std::string channelsName = getArg(command, this->getName() + ' ');
	std::string password = getArg(command, channelsName + ' ');
	if (channelsName.empty())
	{
		std::ostringstream oss;
	    oss << ":ircserv 461" << this->getName() << " :Not enough parameters";
        cli->sendMessage(oss);
        return 0;
    }
	std::vector<std::string> channels = split(channelsName, ',');
	std::vector<std::string> passwords = split(password, ','); 
	for (size_t i = 0; !channels[i].empty(); i++)
		joinChannel(cli, channels[i], passwords[i]);
	return 1;
}
