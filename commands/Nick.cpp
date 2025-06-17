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
		std::cerr << cli.getName() << ": no name given " << std::endl;
	}
	return 1;
}
