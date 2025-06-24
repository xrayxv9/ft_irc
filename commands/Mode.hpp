#pragma once

#include "Command.hpp"

class Mode : public Command
{
	public :
		Mode();
		virtual int execute(const std::string &, Client *) const;

};
