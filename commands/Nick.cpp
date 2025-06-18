#include "Nick.hpp"
#include "../client/Client.hpp"

Nick::Nick(): Command("NICK", "changes the name of the user") {}

int Nick::execute(const std::string &command, Client &cli) const
{
	std::map<int, Client *> clients = cli.getServer().getClients();
	std::string nick;
	int x = 5;

	for (;command[x] && command[x] == ' '; x++);

	if (!command[x])
	{
		std::cerr << cli.getName() << ": no nickname given " << std::endl;
		return 0;
	}
	for (; command[x] && std::isalnum(command[x]); x++)
	{
		nick += command[x];		
	}
	if (command[x] && !std::isalnum(command[x]))
	{
		std::cerr << cli.getName() << ": Erroneus Nickname" << std::endl;
		return 0;
	}

	for (std::map<int, Client *>::iterator it = cli.getServer().getClients().begin(); it != cli.getServer().getClients().end(); it++)
	{
		if (it->second->getName() == nick)
		{
			std::cerr << cli.getName() << ": nickname in use" << std::endl;
			return 0;
		}
	}

	std::ostringstream oss;
	oss <<  ':' << cli.getName() << "!realname@ircserv NICK :" << nick;
	cli.sendMessage(oss);



	return 1;
}
