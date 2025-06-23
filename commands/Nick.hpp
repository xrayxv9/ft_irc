#pragma once

#include "Command.hpp"

class Client;

class Nick : public Command
{
	public :
		Nick();

		int execute(const std::string &command, Client *cli) const;
};
