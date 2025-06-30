#include "Server.hpp"
#include "Invite.hpp"
#include "Kick.hpp"
#include "Names.hpp"
#include "PrivMSG.hpp"
#include "Topic.hpp"
#include "Who.hpp"
#include <Join.hpp>
#include <exception>
#include <map>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>

Server::Server( int port , std::string passwd )
{
	// save password
	int opt = 1;
	
	this->_passwd = passwd;
	std::cout << this->_passwd << std::endl;
	// server socket handle
	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	// poll init
	createFd( this->socketFd );

	// binding
	if (bind(this->socketFd, (sockaddr *)&hint, sizeof(hint)) == -1)
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

	//Registering commands
	this->commands["PRIVMSG"] = new PrivMSG();
	this->commands["NICK"] = new Nick();
	this->commands["JOIN"] = new Join();
	this->commands["pass"] = new Password();
	this->commands["WHO"] = new Who();
	this->commands["KICK"] = new Kick();
	this->commands["TOPIC"] = new Topic();
	this->commands["INVITE"] = new Invite();
	this->commands["NAMES"] = new Names();
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
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
		delete it->second;
	close(this->socketFd);
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

std::string getArg(std::string input, std::string toFind, bool skipSpace)
{
	int where = input.find(toFind);
	std::string res = "";
	if (where == -1)
		return std::string("");
	where += toFind.length();
	for(; input[where] >= 32 + !skipSpace && input[where] <= 126; where ++)
		res += input[where];
	return res;
}

void Server::executeCommand()
{
	int result;
	char reading[1024] = {0};
	std::string str_reading;
	int commandFound= 0;

	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		if (it->revents & POLLIN && it.base() != fds.data())
		{
			result = recv(it->fd, reading, sizeof(reading) - 1, 0);
			if (result)
			{
				for (int x = 0; reading[x] && reading[x] != ' ' && reading[x] != '\r' && reading[x] != '\n'; x++)
					str_reading += reading[x];
				for (std::map<std::string, Command *>::iterator at = commands.begin(); at != commands.end(); at++)
					if (at->first == str_reading)
					{
						if ( this->clients[it->fd]->isRegistered() || str_reading == "pass" )
						{
							commandFound = 1;
							this->commands[str_reading]->execute(reading, this->clients[it->fd]);
						}
						else
						{
							this->clients[it->fd]->sendMessage("You are not logged in, please use /pass <password>");
							return ;
						} 
					}
				if (!commandFound)
					std::cout << "Command not found, the command was : " << str_reading << std::endl;
				std::cout << "Reading is: " << reading << std::endl;
				std::cout << "___________________________________________________________" << std::endl;
			}
			else
			{
				std::cout << "left" << std::endl;
				Client *cli;
				cli = clients[it->fd];
				clients.erase(it->fd);
				delete cli;
				fds.erase(it);
				it--;
			}
			it->revents = 0;
		}
	}


}

void Server::run()
{
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientFd;
	char reading[1024];
	Client *clientClass;

	std::string welcomeMessage = "Welcome to the brand new onlyFans Server";
	
	while (g_running)
	{
		poll(fds.data(), fds.size(), 10);
		if (fds.data()->revents & POLLIN)
		{
			clientFd = accept(this->socketFd, (sockaddr *)&client, &clientSize);
			recv(clientFd, reading, sizeof(reading), 0);
			std::string username = getArg(std::string(reading), "USER ");
			std::string nickname = getArg(std::string(reading), "NICK ");
			if (username.empty() || nickname.empty())
			{
				close(clientFd);
				std::cout << "Invalid session tried to connect" << std::endl;
				continue;
			}
			std::cout << "-----" << clientFd << "-----" << reading << "----------" << std::endl;
			clientClass = new Client(clientFd, getIndexClient(), *this, username, nickname);
			clientClass->sendMessage(welcomeMessage);
			createFd( clientFd );
			clients[clientFd] = clientClass;
			fds.data()->revents = 0;
		}
		else
		{
			executeCommand();
		}
	}
}

std::map<int, Client *> &Server::getClients()
{
	return clients;
}

std::string Server::getPasswd()
{
	return _passwd;
}

Client *Server::getClientByString(const std::string &toFind)
{
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->second->getUserName() == toFind)
			return it->second;
	return NULL;
}
