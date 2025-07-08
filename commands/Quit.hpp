#pragma once

#include "Command.hpp"
class Quit: public Command
{
	public:
		Quit();
		~Quit();
		int execute(const std::string &, Client *cli) const;

};
