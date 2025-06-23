#include "Server.hpp"
#include "Who.hpp"
#include <Join.hpp>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>

Server::Server( int port )
{
	// server socket handle
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// poll init
	createFd( socketFd );

	// binding
	if (bind(socketFd, (sockaddr *)&hint, sizeof(hint)) == -1)
	{
		this->success = 0;
		std::cerr << "Couldn't bind the server" << std::endl;
		return ;
	}
	// listening
	if (listen(socketFd, SOMAXCONN) == -1)
	{
		this->success = 0;
		std::cerr << "can't listen" << std::endl;
		return ;
	}
	//fcntl
	if (fcntl(this->socketFd, O_NONBLOCK))

	//Registering commands
	this->commands["join"] = new Join();
	this->commands["who"] = new Who();
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

std::map<std::string, Channel *> &Server::getChannels()
{
	return this->channels;
}

Server::~Server() 
{
	close(this->socketFd);
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
		delete it->second;
	for (std::map<std::string, Command *>::iterator it = commands.begin(); it != commands.end(); it++)
		delete it->second;
	for (std::map<std::string, Channel *>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		delete it->second;
	std::cout << "server closed" << std::endl;
}

int Server::getIndexClient()
{
	int i = 0;
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
		i++;
	return (i);
}

std::string getArg(std::string input, std::string toFind)
{
	int where = input.find(toFind);
	std::string res = "";
	if (where == -1)
		return std::string("");
	where += toFind.length();
	for(; input[where] >= 33 && input[where] <= 126; where ++)
		res += input[where];
	std::cout << std::endl << "Here is: " << (int)input[where + 1] << std::endl;
	return res;
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
			recv(clientFd, reading, sizeof(reading), 0);
			std::cout << "----------" << reading << "----------" << std::endl;
			std::string username = getArg(std::string(reading), "USER ");
			std::string nickname = getArg(std::string(reading), "NICK ");
			if (username.empty() || nickname.empty())
			{
				close(clientFd);
				std::cout << "Invalid session tried to connect" << std::endl;
				continue;
			}
			clientClass = new Client(clientFd, getIndexClient(), *this, nickname, username);
			send(clientClass->getFd(), welcomeMessage.c_str(), welcomeMessage.length(), 0);
			createFd( clientFd );
			clients[clientFd] = clientClass;
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
					{
						if (std::string(reading).rfind("JOIN ") == 0)
							this->commands["join"]->execute(reading, this->clients[it->fd]);
						else if (std::string(reading).rfind("WHO ") == 0)
							this->commands["who"]->execute(reading, this->clients[it->fd]);
						std::cout << "Reading is: " << reading << std::endl;
					}
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

std::map<int, Client *> &Server::getClients()
{
	return clients;
}
