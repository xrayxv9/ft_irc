#pragma once

#include <vector>

#include "Command.hpp"

class Mode : public Command
{
	public :
		Mode();
		virtual int execute(const std::string &, Client *) const;
	private:
		void iCommand(bool) const;
		void kCommand(std::string, bool) const;
		void tCommand(bool) const;
		void oCommand(bool) const;
		void lCommand(bool) const;
		std::vector<std::string> getAllArgs(std::string  &command) const;
	
		
};
