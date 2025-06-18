#include "Channel.hpp"
#include "../client/Client.hpp"
#include <sys/socket.h>
#include <vector>

Channel::Channel(std::string channelName): _channelName(channelName) {}

Channel::Channel(std::string channelName, Client &client): _channelName(channelName)
{
	_clientList.push_back(client);
}

void Channel::sendMessage(const std::string &message)
{
	for (std::vector<Client>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); it++)
		send(it->getFd(), message.c_str(), message.size(), 0);
}

Channel::~Channel()
{}	
