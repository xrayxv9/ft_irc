#include "User.hpp"
#include "../client/Client.hpp"

User::User(): Command("NICK", "changes the name of the user") {}

int User::execute(const std::string &command, Client *cli) const
{
	std::map<int, Client *> clients = cli->getServer().getClients();
	std::string nick;
	int x = 5;

	for (;command[x] && command[x] == ' '; x++);

	if (!command[x])
	{
		std::cerr << cli->getUserName() << ": no nickname given " << std::endl;
		return 0;
	}
	for (; command[x] && std::isalnum(command[x]); x++)
		nick += command[x];
	std::cout << command << std::endl;
	std::cout << nick << std::endl;
	if (command[x] != '\r' && !std::isalnum(command[x]) && command[x] != ' ')
	{
		std::cerr << cli->getUserName() << ": Erroneus Username" << std::endl;
		return 0;
	}

	for (std::map<int, Client *>::iterator it = cli->getServer().getClients().begin(); it != cli->getServer().getClients().end(); it++)
	{
		if (it->second && it->second->getUserName() == nick)
		{
			std::cerr << cli->getUserName() << ": nickname in use" << std::endl;
			return 0;
		}
	}
	std::ostringstream oss;
	cli->setUserName(nick);
	cli->sendMessage(oss);
	return 1;
}
