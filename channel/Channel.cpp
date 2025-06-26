#include "Channel.hpp"
#include "../client/Client.hpp"
#include <algorithm>
#include <ctime>
#include <sys/socket.h>
#include <vector>

Channel::Channel(std::string channelName, std::string password): _channelName(channelName), _password(password) {}

Channel::Channel(std::string channelName, std::string password, Client *client): _channelName(channelName), _password(password)
{
	_clientList.push_back(client);
	std::cout << "Creating channel with " << channelName << std::endl;
}

void Channel::sendMessage(const std::string &message)
{
	for (std::vector<Client *>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); it++)
		send((*it)->getFd(), message.c_str(), message.size(), 0);
}

std::vector<Client *> &Channel::getClients()
{
	return this->_clientList;
}

std::string Channel::getName()
{
	return this->_channelName;
}	

void Channel::kick(Client *cli)
{
	std::vector<Client *>::iterator it = std::find(this->_clientList.begin(), this->_clientList.end() + 2, cli);
	if (it == this->_clientList.end())
		return ;
	this->_clientList.erase(it);
}

const std::string &Channel::getTopic() const
{
	return this->_topic;
}

void Channel::setTopic(const std::string &topic, const std::string &who)
{
	this->_topic = topic;
	this->_whoSetTopic = who;
	std::time(&this->_ts);
}

const time_t &Channel::lastUpate() const
{
	return this->_ts;
}

const std::string &Channel::whoSetTopic() const
{
	return this->_whoSetTopic;
}

const std::string &Channel::whoSetTopicMask() const
{
	return this->_whoSetTopicMask;
}

void Channel::setTopicMask(const std::string &mask)
{
	this->_whoSetTopicMask = mask;
}

const std::string &Channel::getPassword() const
{
	return this->_password;
}

Channel::~Channel()
{}
