#include "Password.hpp"
#include "../client/Client.hpp"

Password::Password(): Command("PASS", "Allows you to connect to the server with a password") {}

int Password::execute(const std::string &command, Client &cli) const
{
	std::string passwd;
	int i = 5;

	for (; command [i] && command[i] == ' '; i++);

	if (cli.isRegistered())
	{
		cli.sendMessage("You may not register");
		return 0;
	}
	if (!command[i] || command[i] == '\r' || command[i] == '\n')
	{
		cli.sendMessage("Not enough arguments");
		return 0;
	}

	for (; command[i] && command[i] != '\r' && command[i] != '\n'; i++)
		passwd += command[i];

	if (passwd == cli.getServer().getPasswd())
	{
		std::ostringstream oss;

		cli.sendMessage(oss);
		cli.logIn();
		return 1;
	}
	cli.sendMessage("Incorrect password");
	return 0;
}
