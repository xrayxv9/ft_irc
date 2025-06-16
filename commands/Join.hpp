#pragma once

#include "Command.hpp"

class Client;

class Join: public Command
{
	public:
		Join();
		int execute(const std::string &command, const Client &cli) const;

};
