#pragma once

#include <map>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
// #include <Client.hpp>
#include "../client/Client.hpp"

class Client;

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
		int success;
		int socketFd;
		sockaddr_in hint;
		std::map<std::string, int> channels;
		std::map<int, Client *> clients;
		
};
