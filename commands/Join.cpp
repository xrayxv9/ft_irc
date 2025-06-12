#include "Join.hpp"
#include <iostream>

Join::Join(): Command("Join", "Join a channel")
{

}

int Join::execute(const std::string &command) const
{
	std::cout << "Hello world" << std::endl;
	return 1;
}
