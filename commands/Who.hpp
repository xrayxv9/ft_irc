#pragma once

#include "Command.hpp"

class Client;

class Who: public Command
{
	public:
		Who();
		~Who();
		int execute(const std::string &command, Client &cli) const;
};
