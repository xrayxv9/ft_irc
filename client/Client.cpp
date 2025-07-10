#include "Client.hpp"
#include "../channel/Channel.hpp"
#include <map>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <main.hpp>

Client::Client( int fd, Server &server):
	_server(server),
	_clientFd(fd)
{
	_isRegistered = false;

	this->_rank = USER;
}

Client::~Client()
{
	std::cout << "Deleting client " << this->_userName << std::endl;
	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
	{
		it->second->removeClient(this);
		std::cout << "		- Removing from channel " << it->second->getName() << std::endl;
	}
	close (this->_clientFd);
}

std::string Client::generateMask()
{
	std::string res = "";
	res += this->_nickName;
	res += '!';
	res += this->_userName;
	res += "@127.0.0.1";

	return res;
}

int Client::getFd() const
{
	return this->_clientFd;
}

std::map<std::string, Channel *> &Client::getChannels()
{
	return this->_channels;
}

void Client::sendMessage(std::ostringstream &message)
{
	message << "\r\n";
	std::string str = message.str();
	send(this->_clientFd, str.c_str(), str.length(), 0);
}

void Client::sendMessage(std::string str)
{
	str += "\r\n";
	send(this->_clientFd, str.c_str(), str.length(), 0);
}

Channel *Client::joinChannel(const std::string &channelName, const std::string &key)
{
	std::ostringstream oss;
	Channel *channel = NULL;  

	if (this->_server.getChannels().find(channelName) == this->_server.getChannels().end())
	{
		channel = new Channel(channelName, key);
		this->_server.getChannels()[channelName] = channel;
		channel->getModo().push_back(this);
	}
	else
	{
		channel = this->_server.getChannels()[channelName];
		if (channel->getMode().find('k') != std::string::npos && key != channel->getPassword())
		{
			oss.clear();
			oss << ":ircserv 475 " << _userName << " " << channelName << " :Cannot join channel (+k)";
			sendMessage(oss);
			return NULL;
		}
		if (channel->getMode().find('i') != std::string::npos)
		{
			bool found = false;
			std::vector<Client *> clients = channel->getInvited();
			for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it)->getUserName() == this->_userName)
				{
					found = true;
					clients.erase(it);
					break;
				}
			}
			if (!found)
			{
				oss.clear();
				oss << ":ircserv 473 " << _userName << " " << channelName << " :Cannot join channel (+i)";
				sendMessage(oss);
				return NULL;
			}
		}
		if (!channel->getPassword().empty() && channel->getPassword() != key)
			return NULL;
	}
	{
		if ((unsigned long)channel->getUserLimit() >= channel->getClients().size())
		{
			oss << ":ircserv 471 " << _userName << " " << channelName << " :Cannot join channel (+l) - channel is full, try again later";
			sendMessage(oss);
			return NULL;
		}
	}
	oss << ':' << this->generateMask() << " JOIN :" << channelName;
	this->sendMessage(oss);
	this->_channels[channelName] = channel;
	return channel;
}

Server &Client::getServer()
{
	return _server;
}

std::string Client::getNickName() const
{
	return _nickName;
}

int Client::isRegistered() const
{
	return _isRegistered && !_userName.empty() && !_nickName.empty();
}

void Client::logIn()
{
	_isRegistered = 1;
}

std::string Client::getUserName() const
{
	return this->_userName;
}


void Client::setNickName( std::string nickName )
{
	_nickName = nickName;
}

void Client::setUserName( std::string userName )
{
	_userName = userName;
}

std::vector<std::string> &Client::getQueue()
{
	return this->_queue;
}

int Client::updateQueue()
{
	char reading[1024];
	int result = recv(this->_clientFd, reading, 1024, 0);
	if (result > 0)
	{
		std::vector<std::string> splitted = split(std::string(reading), '\n');
		for (std::vector<std::string>::iterator it = splitted.begin(); it != splitted.end(); it++)
		{
			if ((*it)[(*it).size() - 1] == '\r')
			{
				this->_queue.push_back(this->_buff + *it);
				this->_buff.clear();
			}
			else
				this->_buff = *it;
		}
	}
	else
		return 1;
	return 0;
}


bool Client::isMod(Channel *channel) const
{
	for (std::vector<Client *>::iterator it = channel->getModo().begin(); it != channel->getModo().end(); it++)
	{
		if (*it == this)
			return true;
	}
	return false;
}

bool Client::isInChannel(Channel *channel) const
{
	for (std::vector<Client *>::iterator it = channel->getClients().begin(); it != channel->getClients().end(); it++)
	{
		if (*it == this)
			return true;
	}
	return false;
}

