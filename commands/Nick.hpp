#pragma once

#include "Command.hpp"

class Client;

class Nick : public Command
{
	public :
		Nick();
		int execute(const std::string &command, Client *cli) const;
		
		class	ERR_ERRONEUSNICKNAME : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	ERR_NICKNAMEINUSE: public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};
