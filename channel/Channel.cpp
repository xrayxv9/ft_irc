#include "Channel.hpp"
#include "../client/Client.hpp"
#include <algorithm>
#include <sys/socket.h>
#include <vector>

Channel::Channel(std::string channelName): _channelName(channelName) {}

Channel::Channel(std::string channelName, Client *client): _channelName(channelName)
{
	_mode = "";
	_inviteOnly = false;
	_isAdminRestricted = true;
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

Channel::~Channel()
{}

std::vector<Client *> &Channel::getModo()
{
	return _modoList;
}

void Channel::setInviteOnly( bool inviteOnly )
{
	if ( inviteOnly )
		sendMessage("127.0.0.1 ");
	_inviteOnly = inviteOnly;
}

std::string &Channel::getMode()
{
	return _mode;
}

bool Channel::isRestricted()
{
	return _isAdminRestricted;
}

