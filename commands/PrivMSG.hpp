#pragma once

#include <Command.hpp>
#include <string>

class Client;

class PrivMSG: public Command
{
	public:
		PrivMSG();
		~PrivMSG();
		int execute(const std::string &, Client *) const;
};
