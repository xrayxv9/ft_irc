#include "Channel.hpp"
#include "../client/Client.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>

Channel::Channel(std::string channelName, std::string password): _channelName(channelName), _password(password) {}

Channel::Channel(std::string channelName, std::string password, Client *client): _channelName(channelName), _password(password)
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
	std::cout << "is plus or minus : " << (inviteOnly? "+" : "-")<< std::endl;
	std::ostringstream oss;

	setMode('i', inviteOnly, client, "");
	_inviteOnly = inviteOnly;
}

void Channel::restrictTopic( bool restrict, Client *cli )
{
	std::ostringstream oss;

	setMode('t', restrict, cli, "");
	_isAdminRestricted = restrict;
}

void Channel::setPassword( std::string password, bool passwordBool, Client *cli )
{
	std::ostringstream oss;

	if (password.empty())
	{
		std::cout << "no password given" << std::endl;
		return ;
	}
	_password = password;
	_isPasswordLimited = passwordBool;
	setMode('k', passwordBool, cli, password);
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
	setMode('o', clientToMod, cli, clientToMod->getUserName());
}

void Channel::setUserLimit( bool userLimitBool, int userLimit, Client *cli)
{
	std::ostringstream oss;
	std::string number;
	
	oss << userLimit;
	number = oss.str();
	if (userLimitBool)
		_userLimit = -1;
	else
		_userLimit = userLimit;		
	setMode('l', userLimitBool, cli, number);
}

void Channel::setMode( char letter, bool value, Client *cli, std::string options)
{
	std::ostringstream oss;

	if (_mode.find(letter) != std::string::npos)
		return ;
	if (value)
	{
		std::cout << "value positive" << std::endl;
		if (_mode.find(letter) == std::string::npos)
		{
			std::cout << "ajout d'une lettre" << std::endl;
			_mode += letter;
		}
	}
	else
	{
		std::cout << "Value negative" << std::endl;
		if (_mode.find(letter) != std::string::npos)
		{
			std::cout << "retrait d'une lettre " << std::endl;
			_mode.erase(_mode.find(letter));
		}
	}
	std::cout << "mode : " << _mode << std::endl;
	oss << ":" << cli->generateMask() << " MODE " << _channelName << " " << (value? "+" : "-") << letter << (!options.empty()? " " : "") << options;
	cli->sendMessage(oss);
}

void Channel::setTimeStamp()
{
	std::time(&this->_tsMode);
}

time_t Channel::getTimeStamp()
{
	return _tsMode;
}

void Channel::removeClient(Client *client)
{
	for (std::vector<Client *>::iterator it = this->_clientList.begin(); it != this->_clientList.end(); it ++)
		if (*it == client)
		{
			this->_clientList.erase(it);
			break;
		}
}
