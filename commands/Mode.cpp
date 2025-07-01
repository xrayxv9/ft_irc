#include "Mode.hpp"
#include "Server.hpp"
#include <Client.hpp>
#include <cctype>
#include <cstddef>
#include <map>
#include <sstream>

Mode::Mode(): Command("MODE", "changes the mode of the current channel") {}

std::string handleOpts(std::string &arg, int &i)
{
	std::string opts;
	for (int x = 0; arg[i] && (std::isalpha(arg[i]) || arg[i] == '+' || arg[i] == '-'); i++, x++)
	{
		if (arg[i] == 'a')
		{
			opts[x] = arg[i];
		}
	}
	return ("coucou");
}

int Mode::execute(const std::string &command, Client *cli) const
{
	std::string channelName;
	std::string options;
	std::ostringstream oss;
	std::string arg = getArg(command, "MODE ", true);
	std::vector<std::string> args = getAllArgs(arg);
	std::vector<std::string>::iterator it = args.begin();
	size_t iter = 1;
	bool isAdmin = false;
	char mode = '+';

	channelName = *it;
	Channel *channelNeeded = NULL;
	std::map<std::string, Channel *> channels = cli->getChannels();
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->first == channelName)
		{
			channelNeeded = it->second;
			std::vector<Client *> modos = it->second->getModo(); 
			for (std::vector<Client *>::iterator at = modos.begin(); at != modos.end(); at++)
			{
				if (cli == *at)
				{
					isAdmin = true;
				}
			}
		}
	}
	if (!channelNeeded)
	{
		cli->sendMessage("The channel doesn't exist");
		return 0;
	}
	if (!isAdmin && channelNeeded->isRestricted())
	{
		cli->sendMessage("You are not an admin");
		return 0;
	}

	it++;
	if (it == args.end())
	{
		oss << ":ircserv" << " 324 " << channelNeeded->getName() << " +" << channelNeeded->getMode();
		cli->sendMessage(oss);
		return 0;
	}
	// if an unknown char was found
	for (int i = 0; (*it)[i] ;i++)
	{
		if ((*it)[i] == '+')
			mode = '+';
		else if ((*it)[i] == '-')
			mode = '-';
		else if ((*it)[i] == 'i')
			iCommand(mode == '+', channelNeeded, cli);
		else if ((*it)[i] == 'k')
		{
			if (args.size() > iter)
				kCommand(*(it + iter++), mode == '+', channelNeeded, cli);
		}
		else if ((*it)[i] == 'l')
		{
			// TODO ADD THE VERIF FOR THE INTS HERE !
			lCommand(1, mode == '+', channelNeeded, cli);
		}
		else if ((*it)[i] == 'o')
		{
			if (args.size() > iter)
			{
				std::vector<Client *> clients = channelNeeded->getClients();
				std::vector<Client *>::iterator at = clients.begin();
				for (; at != clients.end(); at++)
				{
					if (*(it + iter) == (*at)->getNickName())
						break;
				}
				oCommand(mode == '+', channelNeeded, *at, cli);
			}
			oss << ":" << cli->generateMask() << " :No such nick/channel";
			cli->sendMessage(oss);
		}
		else if ((*it)[i] == 't')
			tCommand(mode == '+', channelNeeded, cli);
		else if (!isalpha((*it)[i]))
		{
			std::cout << *it << std::endl;
			cli->sendMessage("Unknown character");
			return (1);
		}
	}
	return 1;
}

std::vector<std::string> Mode::getAllArgs(std::string &command) const
{
	std::vector<std::string> args;
	std::string opt = "";
	for (size_t x = 0; x < command.length(); x++)
	{
		while (command[x] && command[x] == ' ')
			x++;
		while (command[x] && (command[x] != ',' && command[x] != ' ' && command[x] >= ' '))
			opt += command[x++];
		if (!opt.empty())
			args.push_back(opt);
		opt = "";
	}
	return (args);
}

void Mode::iCommand(bool cond, Channel *channel, Client *cli ) const
{
	channel->setInviteOnly(cond, cli);
}

void Mode::kCommand(std::string password, bool cond, Channel *channel, Client * cli ) const
{
	channel->setPassword(password, cond, cli);
}

void Mode::lCommand(int limit, bool cond, Channel *channel, Client * cli ) const
{
	channel->setUserLimit(cond, limit, cli);
}
void Mode::oCommand(bool cond, Channel *channel, Client * clientToMod, Client *cli) const
{
	channel->setModo(clientToMod, cond, cli);
}
void Mode::tCommand(bool cond, Channel *channel, Client * cli ) const
{
	channel->restrictTopic(cond, cli);
}
