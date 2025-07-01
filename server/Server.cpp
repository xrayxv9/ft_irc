#include <Server.hpp>
#include "Invite.hpp"
#include "Kick.hpp"
#include "Names.hpp"
#include "PrivMSG.hpp"
#include "Mode.hpp"
#include "Topic.hpp"
#include "User.hpp"
#include "Who.hpp"
#include <Join.hpp>
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
	this->commands["PASS"] = new Password();
	this->commands["WHO"] = new Who();
	this->commands["KICK"] = new Kick();
	this->commands["MODE"] = new Mode();
	this->commands["TOPIC"] = new Topic();
	this->commands["INVITE"] = new Invite();
	this->commands["NAMES"] = new Names();
	this->commands["USER"] = new User();
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
	// char reading[1024] = {0};
	std::string command;
	// int commandFound= 0;

	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
	{
		if (it->revents & POLLIN && it.base() != fds.data())
		{
			Client *client = this->clients[it->fd];
			if (client->updateQueue())
			{
				fds.erase(it);
        continue;
			}
			for (std::vector<std::string>::iterator it = client->getQueue().begin(); it != client->getQueue().end(); it++)
			{
				std::cout << "------" << client->getQueue().size() << "------"<< std::endl;
				command = "";
				for (int x = 0; (*it)[x] != '\0' && (*it)[x] != ' ' && (*it)[x] != '\r' && (*it)[x] != '\n'; x++)
					command += (*it)[x];
				std::cout << "Searching command '" << command << '\'' << std::endl;
				if (this->commands[command] == NULL)
				{
					//Invalid command
					std::cout << "Invalid command '" << command << '\'' << std::endl;
					continue ;
				}
				if (client->isRegistered() || command == "USER" || command == "NICK" || command == "PASS")
					this->commands[command]->execute(*it, client);
				else
					client->sendMessage("You are not logged in, please use /pass <password>");
				client->getQueue().erase(it);
				it--;
				// std::cout << "Reading is: " << command << std::endl;
				std::cout << "___________________________________________________________" << std::endl;
			}
		}
		it->revents = 0;
	}
}

void Server::run()
{
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientFd;
	// char reading[1024];
	Client *clientClass;

	std::string welcomeMessage = "Welcome to the brand new onlyFans Server";
	
	while (g_running)
	{
		poll(fds.data(), fds.size(), 10);
		if (fds.data()->revents & POLLIN)
		{
			clientFd = accept(this->socketFd, (sockaddr *)&client, &clientSize);
			clientClass = new Client(clientFd, getIndexClient(), *this);
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
