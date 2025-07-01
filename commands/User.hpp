#pragma once

#include "Command.hpp"

class Client;

class User : public Command
{
	public :
		User();

		int execute(const std::string &command, Client *cli) const;
};
