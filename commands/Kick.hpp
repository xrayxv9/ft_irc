#pragma once

#include <Command.hpp>
#include <string>

class Client;

class Kick: public Command
{
	public: 
		Kick();
		~Kick();
		int execute(const std::string &, Client *) const;
};
