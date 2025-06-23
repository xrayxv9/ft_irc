#include "Client.hpp"
#include "../channel/Channel.hpp"
#include <sstream>

Client::Client( int fd, int index, Server &server, std::string &userName, std::string &nickName):
	_server(server),
	_clientFd(fd),
	_nickName(nickName),
	_userName(userName)
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
	std::string message;

	send(this->_clientFd, message.c_str(), message.length(), 0);
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
	message << '\n';
	std::string str = message.str();
	send(this->_clientFd, str.c_str(), str.length(), 0);
}

void Client::sendMessage(std::string str)
{
	str += '\n';
	send(this->_clientFd, str.c_str(), str.length(), 0);
}

Channel *Client::joinChannel(const std::string &channelName)
{
	std::ostringstream oss;
	oss << ':' << this->generateMask() << " JOIN :" << channelName;
	this->sendMessage(oss);
	Channel *channel;  
	if (this->_server.getChannels().find(channelName) == this->_server.getChannels().end())
	{
		channel = new Channel(channelName);
		this->_server.getChannels()[channelName] = channel;
	}
	else 
		channel = this->_server.getChannels()[channelName];
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
	return _isRegistered;
}

void Client::logIn()
{
	_isRegistered = 1;
}

std::string Client::getUserName() const
{
	return this->_userName;
}
