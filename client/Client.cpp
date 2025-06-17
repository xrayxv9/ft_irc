#include "Client.hpp"
#include <sstream>

Client::Client( int fd, int index, Server &server): _server(server) ,_clientFd(fd)
{
	_currentChannel = new Channel("default", *this);
	std::ostringstream oss;
	std::cout << index << std::endl;
	oss << index;
	std::string str = oss.str(); 

	this->_nickName = "default" + str;
	this->_rank = USER;
}

Client::~Client()
{
	std::string message;

	send(this->_clientFd, message.c_str(), message.length(), 0);
	close (this->_clientFd);
}

int Client::getFd() const
{
	return this->_clientFd;
}

Channel *Client::getCurrentChannel()
{
	return this->_currentChannel;
}

void Client::sendMessage(std::ostringstream &message)
{
	message << '\n';
	std::string str = message.str();
	send(this->_clientFd, str.c_str(), str.length(), 0);
}

void Client::joinChannel(const std::string &channel)
{
	std::ostringstream oss;
	if (channel.find(' ') || channel[0] != '#')
		oss << "Bad channel name";
	else
		oss << ':' << this->_nickName << "!realname@ircserv JOIN :" << channel;
	this->sendMessage(oss);
}

Server &Client::getServer()
{
	return _server;
}
