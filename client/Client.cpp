#include "Client.hpp"
#include "../channel/Channel.hpp"
#include <map>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <main.hpp>

Client::Client( int fd, int index, Server &server):
	_server(server),
	_clientFd(fd)
{
	_isRegistered = false;
	std::ostringstream oss;
	std::cout << index << std::endl;
	oss << index;
	std::string str = oss.str(); 

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
	oss << ':' << this->generateMask() << " JOIN :" << channelName;
	this->sendMessage(oss);
	Channel *channel;  
	if (this->_server.getChannels().find(channelName) == this->_server.getChannels().end())
	{
		channel = new Channel(channelName, key);
		this->_server.getChannels()[channelName] = channel;
		channel->getModo().push_back(this);
	}
	else
	{
		channel = this->_server.getChannels()[channelName];
		if (!channel->getPassword().empty() && channel->getPassword() != key)
			return NULL;
	}

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
	{
		std::cout << "left" << std::endl;
		return 1;
	}
	return 0;
}
