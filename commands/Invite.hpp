#pragma once

#include <Command.hpp>

class Client;

class Invite: public Command
{
	public:
		Invite();
		~Invite();
		int execute(const std::string &, Client *) const;
};
