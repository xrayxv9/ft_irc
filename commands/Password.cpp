#include "Password.hpp"
#include "../client/Client.hpp"
#include <sstream>

Password::Password(): Command("PASS", "Allows you to connect to the server with a password") {}

int Password::execute(const std::string &command, Client *cli) const
{
	std::string passwd;
	int i = 5;

	for (; command [i] && command[i] == ' '; i++);

	if (cli->isRegistered())
	{
		cli->sendMessage("You may not register");
		return 0;
	}
	if (!command[i] || command[i] == '\r' || command[i] == '\n')
	{
		std::ostringstream oss;
	    oss << ":ircserv 461" << this->getName() << " :Not enough parameters";
        cli->sendMessage(oss);
		return 0;
	}

	for (; command[i] && command[i] != '\r' && command[i] != '\n'; i++)
		passwd += command[i];

	if (passwd == cli->getServer().getPasswd())
	{
		std::ostringstream oss;
		cli->sendMessage(oss);
		cli->logIn();
		return 1;
	}
	std::ostringstream oss;

	if (!cli->getNickName().empty())
		oss << cli->getNickName();
	else
		oss << "*";
	oss << " :Password incorrect";
	cli->sendMessage(oss);
	return 0;
}
