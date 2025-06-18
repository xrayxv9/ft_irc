#include "Join.hpp"
#include "../client/Client.hpp"


Join::Join(): Command("Join", "Join a channel")
{

}

int Join::execute(const std::string &command, Client &cli) const
{
	//Parsing

	if (command[5] != '#')
	{
		cli.sendMessage(std::string("Channel name must start with #"));
		return 0;
	}
	std::string channel = "";
	for (int i = 5; command[i] != '\n'; i++)
		channel += command[i];
	cli.joinChannel(std::string(channel.c_str()));
	return 1;
}
