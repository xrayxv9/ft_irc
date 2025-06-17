#include "Join.hpp"
#include "../client/Client.hpp"


Join::Join(): Command("Join", "Join a channel")
{

}

int Join::execute(const std::string &command, Client &cli) const
{
	std::cout << "Channel is: " << &command.c_str()[5];
	cli.joinChannel(std::string(&command.c_str()[5]));
	return 1;
}
