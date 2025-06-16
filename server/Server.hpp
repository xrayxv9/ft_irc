#pragma once

#include <map>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <csignal>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <vector>
#include <poll.h>
#include "../client/Client.hpp"

extern bool g_running;

class Client;
class Command;

class Server
{
	public:
		Server( int port );
		~Server();
		int	getFd();
		std::map<std::string, int> getChannels();
		int	getStatus();
		int getIndexClient();
		void run();

	private:
		void createFd( int fd );
		int success;
		int socketFd;
		sockaddr_in hint;
		std::map<std::string, int> channels;
		std::map<int, Client *> clients;
		std::vector<struct pollfd> fds;
		std::map<std::string, Command *> commands;
};
