#include "Nick.hpp"
#include "../client/Client.hpp"

Nick::Nick(): Command("NICK", "changes the name of the user") {}

int Nick::execute(const std::string &command, Client *cli) const
{
	std::map<int, Client *> clients = cli->getServer().getClients();
	std::string nick;
	int x = 5;
	std::cout << "coucou" << std::endl;

	for (;command[x] && command[x] == ' '; x++);

	if (!command[x])
	{
		std::cerr << cli->getNickName() << ": no nickname given " << std::endl;
		return 0;
	}
	for (; command[x] && std::isalnum(command[x]); x++)
	{
		nick += command[x];
	}
	std::cout << command << std::endl;
	std::cout << nick << std::endl;
	if (command[x] != '\r' && !std::isalnum(command[x]))
	{
		std::cerr << cli->getNickName() << ": Erroneus Nickname" << std::endl;
		std::cout << "here is the char : " << std::endl;
		std::cout << command[x] << "."<< std::endl;
		return 0;
	}

	for (std::map<int, Client *>::iterator it = cli->getServer().getClients().begin(); it != cli->getServer().getClients().end(); it++)
	{
		if (it->second->getNickName() == nick)
		{
			std::cerr << cli->getNickName() << ": nickname in use" << std::endl;
			return 0;
		}
	}

	std::ostringstream oss;

	oss <<  ':' << cli->getNickName() << "!realname@ircserv NICK :" << nick;
	cli->sendMessage(oss);
	std::cout << "test" << std::endl;
	return 1;
}
