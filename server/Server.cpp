#include "Server.hpp"
#include <sys/socket.h>

Server::Server( int port )
{
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (bind(socketFd, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		this->success = 0;
		close(socketFd);
		std::cerr << "Couldn't bind the server" << std::endl;
		return ;
	}
	
	if (listen(socketFd, SOMAXCONN) == -1)
	{
		this->success = 0;
		close(socketFd);
		std::cerr << "can't listen" << std::endl;
		return ;
	}
	success = 1;
}

int Server::getStatus()
{
	return success;
}

int Server::getFd()
{
	return this->socketFd;
}

std::map<std::string, int> Server::getChannels()
{
	return this->channels;
}

Server::~Server()
{
	for (std::map<std::string, int>::iterator it = channels.begin(); it != channels.end(); it++)
		close(it->second);
	std::cout << "server closed" << std::endl;
}

void Server::run()
{
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientFd;

	std::string welcomeMessage = "Welcome to the brand new onlyFans Server\n";

	while (true)
	{
		clientFd = accept(this->socketFd, (sockaddr *)&client, &clientSize);
		send(clientFd, welcomeMessage.c_str(), welcomeMessage.length(), 0);
	}
}
