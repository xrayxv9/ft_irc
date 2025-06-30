#pragma once

#include "Command.hpp"
#include <string>

class Client;

class Names: public Command
{
	public:
		Names();
		~Names();
		int execute(const std::string &, Client *) const;
};
