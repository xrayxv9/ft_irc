#include "Channel.hpp"
#include "../client/Client.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <sys/socket.h>
#include <vector>

Channel::Channel(std::string channelName): _channelName(channelName) {}

Channel::Channel(std::string channelName, Client *client): _channelName(channelName)
{
	_mode = "";
	_isPasswordLimited = false;
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

// get

std::vector<Client *> &Channel::getModo() 
{
	return _modoList;
}


std::string &Channel::getMode()
{
	return _mode;
}

bool Channel::isRestricted()
{
	return _isAdminRestricted;
}

bool Channel::isInviteOnly()
{
	return (_inviteOnly);
}

std::string Channel::getPassword()
{
	return (_password);	
}

int Channel::getUserLimit()
{
	return _userLimit;
}

std::vector<Client *>::iterator Channel::isModo( Client *cli )
{
	for (std::vector<Client *>::iterator it = _modoList.begin(); it != _modoList.end(); it++)
	{
		if (*it == cli)
			return it;
	}
	return _modoList.end();
}

// set

void Channel::setInviteOnly( bool inviteOnly, Client *client )
{
	std::ostringstream oss;

	oss << ":" << client->generateMask() << " MODE " << _channelName << " " << (inviteOnly ? "+" : "-") << "i";
	client->sendMessage(oss);
	_inviteOnly = inviteOnly;
}

void Channel::restrictTopic( bool restrict, Client *cli )
{
	std::ostringstream oss;

	oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << (restrict ? "+" : "-") << "i";
	cli->sendMessage(oss);
	_isAdminRestricted = restrict;
}

void Channel::setPassword( std::string password, bool passwordBool, Client *cli )
{
	std::ostringstream oss;

	oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << (passwordBool ? "+" : "-") << "k";
	cli->sendMessage(oss);
	_password = password;
	_isPasswordLimited = passwordBool;
}

void Channel::setModo( Client *clientToMod, bool set, Client *cli )
{
	std::ostringstream oss;

	if (set)
	{
		oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << "+o";
		_modoList.push_back(clientToMod);
	}
	else
	{
		oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << "-o";
		_modoList.erase(isModo(clientToMod));
	}
	cli->sendMessage(oss);
}

void Channel::setUserLimit( bool userLimitBool, int userLimit, Client *cli)
{
	std::ostringstream oss;

	if (userLimitBool)
		_userLimit = -1;
	else
		_userLimit = userLimit;		
	oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << (userLimitBool? "+" : "-") << "l";
	cli->sendMessage(oss);
}
