#include "Client.hpp"
#include <sstream>

Client::Client( int fd, int index, Server &server): _server(server)
{
	(void)_server;
	std::ostringstream oss;
	std::cout << index << std::endl;
	oss << index;
	std::string str = oss.str(); 

	this->_clientFd = fd;
	this->_nickName = "default" + str;
	this->_rank = USER;
}

Client::~Client()
{
	std::string message;

	send(this->_clientFd, message.c_str(), message.length(), 0);
	close (this->_clientFd);
}

Server &Client::getServer()
{
	return _server;
}
