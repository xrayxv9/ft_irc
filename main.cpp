#include <asm-generic/socket.h>
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
#include "Server.hpp"


int main(int argc, char **argv)
{
	std::string portPreConvert;
	std::string passwd;
	int port;
	
	if (argc != 3)
	{
		std::cerr << "Please give at least a port and a password" << std::endl;
		std::cout << "./ft_irc <port> <password>" << std::endl;
		return 1;
	}
	portPreConvert = argv[1];
	if (portPreConvert.length() > 5)
	{
		std::cerr << "The port you have given is incorrect" << std::endl;
		return 1;
	}
	port = std::atoi(portPreConvert.c_str());
	if (port > 65535)
	{
		std::cerr << "The port you have given is incorrect" << std::endl;
		return 1;
	}
	Server *serv = new Server(port);
	if (serv->getStatus() == 0)
	{
		delete serv;
		return 1;
	}
	serv->run();
	delete serv;
}
