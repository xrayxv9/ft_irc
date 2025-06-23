#pragma once

#include "Command.hpp"

class Client;

class Join: public Command
{
	public:
		Join();
		int execute(const std::string &command, Client *cli) const;
};
