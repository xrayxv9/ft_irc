#pragma once

#include <Server.hpp>
#include <Join.hpp>

enum rank
{
	USER,
	MODERATOR,
	ADMIN
};

class Server;

class Client
{
	public:
		Client( int fd, int index, Server &server);
		~Client();
		Server &getServer();

	private:
		Server &_server;
		int _clientFd;
		rank _rank;
		std::string _nickName;
};
