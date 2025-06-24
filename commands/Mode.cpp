#include "Mode.hpp"
#include "Server.hpp"
#include <Client.hpp>

Mode::Mode(): Command("MODE", "changes the mode of the current channel") {}

int Mode::execute(const std::string &command, Client *cli) const
{
	std::string channelName;
	std::string options;
	std::string arg = getArg(command, "MODE ", true);
	int i = 0;
	int x = 0;

	for (; arg[i] && arg[i] != '#'; i++);

	if (!arg[i])
	{
		cli->sendMessage("Please give a channel before anything");
		return 0;
	}
	for (; arg[i] && arg[i] != ' '; i++, x++)
		channelName[x] = arg[i];

	for (; arg[i] && (arg[i] != '+' || arg[i] != '-'); i++);

	for (x = 0;arg[i] && arg[i] != ' ' ; i++, x++)
	{
		options[x] = arg[i];
	}
	
	return 1;
}
