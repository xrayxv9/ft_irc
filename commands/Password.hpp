#pragma once

#include <Command.hpp>

class Password : public Command
{
	public :
		Password();
		int execute(const std::string &command, Client &cli) const;
};
