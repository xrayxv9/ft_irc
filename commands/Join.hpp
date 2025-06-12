#pragma once

#include "Command.hpp"

class Join: public Command
{
	public:
		Join();
		int execute(const std::string &command) const;

};
