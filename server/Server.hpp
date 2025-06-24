#pragma once

#include <map>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <csignal>
#include <iostream>
#include <fcntl.h>
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
#include "../channel/Channel.hpp"
#include "../commands/Nick.hpp"
#include "../commands/Password.hpp"

extern bool g_running;

class Client;
class Command;
class Channel;

class Server
{
	public:
		Server( int port , std::string passwd );
		~Server();
		int	getFd();
		std::map<std::string, Channel *> &getChannels();
		int	getStatus();
		int getIndexClient();
		void executeCommand();
		std::string getPasswd();
		std::map<int, Client *> &getClients();
		void run();
		Client *getClientByString(const std::string &toFind);	

	private:
		void createFd( int fd );
		int success;
		int socketFd;
		sockaddr_in hint;
		void makeCommand(std::string);
		std::map<std::string, Channel *> channels;
		std::map<int, Client *> clients;
		std::vector<struct pollfd> fds;
		std::map<std::string, Command *> commands; 
		std::string _passwd;
};


std::string getArg(std::string input, std::string toFind, bool skipSpace = false);
