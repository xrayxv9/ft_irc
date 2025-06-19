#include "Password.hpp"
#include "../client/Client.hpp"

Password::Password(): Command("PASS", "Allows you to connect to the server with a password") {}

int Password::execute(const std::string &command,Client &cli) const
{
	int x = 5;
	std::string passwd;

	for (;command[x] && command[x] == ' '; x++);

	if (!command[x])
	{
		std::cerr << cli.getName() << " PASS: Not enough parameters" << std::endl;
		return 0;
	}

	if (!cli.isRegistered())
	{
		std::cerr << cli.getName() << " You may not register" << std::endl;
		return 0;
	}

	for (; command[x] && (command[x] != '\r' || command[x] != '\n'); x++)
		passwd += command[x];
	if (passwd == cli.getServer().getPasswd())
	{
		std::ostringstream oss;

		oss <<  ':' << cli.getName() << "!realname@ircserv PASS :" << passwd;
		cli.sendMessage(oss);
		std::cout << "test" << std::endl;
		cli.logIn();
		return 1;
	}
	std::cerr << cli.getName() << ": Incorect password" << std::endl;
	return 0;
}
