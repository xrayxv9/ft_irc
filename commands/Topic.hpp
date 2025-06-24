#pragma once

#include <Command.hpp>

class Client;

class Topic: public Command
{
	public:
		Topic();
		~Topic();
		int execute(const std::string &, Client *) const;
};
