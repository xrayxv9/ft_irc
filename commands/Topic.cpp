#include "Client.hpp"
#include "Server.hpp"
#include <Topic.hpp>
#include <sstream>

Topic::Topic(): Command("TOPIC", "Change or see channel topic")
{

}

Topic::~Topic()
{

}

//TODO: Permission

int Topic::execute(const std::string &command, Client *cli) const
{
	std::string channelName = getArg(command, this->getName() + ' ');
	std::string topic = getArg(command, channelName + " :", true);

	if (channelName.empty())
	{
		std::ostringstream oss;
        oss << ":ircserv " << 461 << " " << this->getName() << " :Not enough parameters";
        cli->sendMessage(oss);
        return 0;
	}
	Channel *channel = cli->getServer().getChannels()[channelName];
    if (channel == NULL)
    {
        std::ostringstream oss;
        oss << ":ircserv " << 403 << " " << channelName << " :No such channel";
        cli->sendMessage(oss);
        return 0;
    }
	if (cli->getChannels()[channelName] == NULL)
    {
        std::ostringstream oss;
        oss << ":ircserv " << 442 << " " << channelName << " :You're not on that channel";
        cli->sendMessage(oss);
        return 0;
    }
	if (channel->getTopic().empty() && topic.empty())
	{
		std::ostringstream oss;
		oss << ":ircserv " << 331 << " " << cli->getUserName() << " " << channelName << " :No topic is set";
		cli->sendMessage(oss);
		return 1;
	}
	if (topic.empty())
	{
		std::ostringstream oss;
		oss << ":ircserv " << 332 << " " << channel->whoSetTopic() << " " << channelName << " :" << channel->getTopic();
		cli->sendMessage(oss);
		std::ostringstream oss2;
		oss2 << ":ircserv " << 333 << " " << channel->whoSetTopic() << " " << channelName << " " << channel->whoSetTopicMask() << " " << channel->lastUpate();
		cli->sendMessage(oss2);
		return 1;
	}
	if (!cli->isMod(channel))
	{
		std::ostringstream oss;
		oss << ":ircserv 482 " << cli->getUserName() << " " << channelName << " :You're not a channel operator";
		cli->sendMessage(oss);
		return 0;
	}
	channel->setTopic(topic, cli->getUserName());
	channel->setTopicMask(cli->generateMask());
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
	{
		std::ostringstream oss;
		oss << ":" << cli->generateMask() << " TOPIC " << channelName << " :" << topic;
		(*it)->sendMessage(oss);
	}
	return 1;
}
