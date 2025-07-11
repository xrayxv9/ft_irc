#include "Password.hpp"
#include "../client/Client.hpp"
#include <sstream>

Password::Password(): Command("PASS", "Allows you to connect to the server with a password") {}

int Password::execute(const std::string &command, Client *cli) const
{
	std::ostringstream oss;
	std::string passwd;
	int i = 5;

	for (; command [i] && command[i] == ' '; i++);

	if (cli->isRegistered())
	{
		oss << ":ircserv 462 " << this->getName() << " :You may not register";
		cli->sendMessage(oss);
		return 0;
	}
	if (!command[i] || command[i] == '\r' || command[i] == '\n')
	{
	    oss << ":ircserv 461" << this->getName() << " :Not enough parameters";
        cli->sendMessage(oss);
		return 0;
	}

	for (; command[i] && command[i] != '\r' && command[i] != '\n'; i++)
		passwd += command[i];

	if (passwd == cli->getServer().getPasswd())
	{
		cli->logIn();
		return 1;
	}

	if (!cli->getNickName().empty())
		oss << cli->getNickName();
	else
		oss << "*";
	oss << " :Password incorrect";
	cli->sendMessage(oss);
	return 0;
}
