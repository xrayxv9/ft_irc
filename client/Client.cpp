#include "Client.hpp"
#include <sstream>

Client::Client( int fd, int index, const Server &server): _server(server)
{
	std::ostringstream oss;
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
