#include "Join.hpp"
#include <iostream>

Join::Join(): Command("Join", "Join a channel")
{

}

int Join::execute(const std::string &command, const Client &cli) const
{
	(void)cli;
	(void)command;
	std::cout << "Hello worldd"  << std::endl;
	return 1;
}
