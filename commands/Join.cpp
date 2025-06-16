#include "Join.hpp"
#include <iostream>
#include "../client/Client.hpp"


Join::Join(): Command("Join", "Join a channel")
{

}

int Join::execute(const std::string &command, const Client &cli) const
{
	(void)command;
	std::string message = "CHANNEL general";
	std::cout << "Hello worlddd: " << cli.getFd() << std::endl;
	send(cli.getFd(), message.c_str(), message.length(), 0);
	return 1;
}
