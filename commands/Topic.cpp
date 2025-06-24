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

//TODO: Permission, RPL_TOPICWHOTIME

int Topic::execute(const std::string &command, Client *cli) const
{
	std::string channelName = getArg(command, this->getName() + ' ');
	std::string topic = getArg(command, channelName + " :", true);

	if (channelName.empty())
	{
		std::ostringstream oss;
        oss << cli->getFd() << " " << this->getName() << " :Not enough parameters";
        cli->sendMessage(oss);
        return 0;
	}
	Channel *channel = cli->getServer().getChannels()[channelName];
    if (channel == NULL)
    {
        std::ostringstream oss;
        oss << cli->getFd() << " " << channelName << " :No such channel";
        cli->sendMessage(oss);
        return 0;
    }
	if (cli->getChannels()[channelName] == NULL)
    {
        std::ostringstream oss;
        oss << cli->getFd() << " " << channelName << " :You're not on that channel";
        cli->sendMessage(oss);
        return 0;
    }
	if (channel->getTopic().empty() && topic.empty())
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << channelName << " :No topic is set";
		cli->sendMessage(oss);
		return 1;
	}
	if (topic.empty())
	{
		std::ostringstream oss;
		oss << cli->getFd() << " " << channelName << " :" << channel->getTopic();
		cli->sendMessage(oss);
		std::ostringstream oss2;
		oss2 << cli->getFd() << " " << channelName << " " << channel->whoSetTopic() << " " << channel->lastUpate(); 
		cli->sendMessage(oss2);
		return 1;
	}
	channel->setTopic(topic, cli->getUserName());
	return 1;
}
