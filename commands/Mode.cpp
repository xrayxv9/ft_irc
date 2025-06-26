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
	std::string arg = getArg(command, "MODE ", true);
	std::vector<std::string> args = getAllArgs(arg);
	std::vector<std::string>::iterator it = args.begin();
	bool isAdmin = false;
	char mode = '+';

	if ((*it)[0] != '#')
	{
		cli->sendMessage("no channel given");
		return 0;
	}

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
	// if an unknown char was found
	for (int i = 0; (*it)[i] ;i++)
	{
		if ((*it)[i] == '+')
			mode = '+';
		else if ((*it)[i] == '-')
			mode = '-';
		if ((*it)[i] == 'i')
			iCommand(mode == '+');
		else if ((*it)[i] == 'k')
			kCommand(*(it + 1), mode == '+');
		else if ((*it)[i] == 'l')
			lCommand(mode == '+');
		else if ((*it)[i] == 'o')
			oCommand(mode == '+');
		else if ((*it)[i] == 't')
			tCommand(mode == '+');
		else if (!isalpha((*it)[i]))
		{
			cli->sendMessage("Unknown character");
			return (0);
		}
	}
	return 1;
}

std::vector<std::string> Mode::getAllArgs(std::string &command) const
{
	std::vector<std::string> args;
	std::string opt = "";
	for (int x = 0; !command[x]; x++)
	{
		for (; command[x] && command[x] != ' ' ; x++);
		for (; command[x] && (command[x] != ',' && command[x] != ' ' && command[x] >= ' '); x++)
		{
			opt += command[x];
		}
		if (!opt.empty())
			args.push_back(opt);
		opt = "";
	}
	return (args);
}
