#include "Client.hpp"
#include <Quit.hpp>
#include <map>

Quit::Quit(): Command("QUIT", "When a user leave the server")
{}

Quit::~Quit()
{}

int Quit::execute(const std::string &command, Client *cli) const
{
	(void)command;
	for (std::map<int, Client *>::iterator it = cli->getServer().getClients().begin(); it != cli->getServer().getClients().end(); it++)
		if (it->first == cli->getFd())
		{
			cli->getServer().getClients().erase(it);
			break ;
		}
	delete cli;
	return 1;
}
