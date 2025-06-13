#include "Server.hpp"
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>


Server::Server( int port )
{
	// server socket handle
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// poll init
	createFd( socketFd );

	// binding
	if (bind(socketFd, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		this->success = 0;
		close(socketFd);
		std::cerr << "Couldn't bind the server" << std::endl;
		return ;
	}
	// listening
	if (listen(socketFd, SOMAXCONN) == -1)
	{
		this->success = 0;
		close(socketFd);
		std::cerr << "can't listen" << std::endl;
		return ;
	}
	success = 1;
}

void Server::createFd( int fd )
{
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
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

	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		delete it->second;
	}

	close(this->socketFd);
	std::cout << "server closed" << std::endl;
}

int Server::getIndexClient()
{
	int i = 0;
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
		i++;
	return (i);
}

void Server::run()
{
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientFd;
	char reading[1024];
	Client *clientClass;
	int result;

	std::string welcomeMessage = "Welcome to the brand new onlyFans Server\n";
	
	while (g_running)
	{
		poll(fds.data(), fds.size(), 10);
		if (fds.data()->revents & POLLIN)
		{
			clientFd = accept(this->socketFd, (sockaddr *)&client, &clientSize);
			send(clientFd, welcomeMessage.c_str(), welcomeMessage.length(), 0);
			clientClass = new Client(clientFd, getIndexClient(), *this);
			createFd( clientFd );
			clients[getIndexClient()] = clientClass;
			fds.data()->revents = 0;
		}
		else
		{
			for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
			{
				if (it->revents & POLLIN)
				{
					result = recv(it->fd, reading, sizeof(reading), 0);
					if (result)
						std::cout << "recv : " << reading << std::endl;
					else
					{
						std::cout << "left" << std::endl;
						clients.erase(it->fd);
						fds.erase(it);
						it--;
					}
					it->revents = 0;
				}
			}
		}
	}
}
