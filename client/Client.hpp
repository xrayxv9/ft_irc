#pragma once

#include <Server.hpp>
#include <Join.hpp>
#include "../channel/Channel.hpp"

enum rank
{
	USER,
	MODERATOR,
	ADMIN
};

class Server;
class Client;

class Client
{
	public:
		Client( int fd, int index, Server &server);
		~Client();
		Server &getServer();
		int getFd() const;
		Channel *getCurrentChannel();
	private:
		Channel *_currentChannel;
		Server &_server;
		int _clientFd;
		rank _rank;
		std::string _nickName;
};
