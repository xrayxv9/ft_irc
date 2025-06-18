#pragma once

#include <Server.hpp>
#include <Join.hpp>
#include <map>
#include <sstream>
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
		Client( int fd, int index, Server &server, std::string &realName);
		~Client();
		Server &getServer();
		int getFd() const;
		Channel *getCurrentChannel();
		void joinChannel(const std::string &);
		void sendMessage(std::ostringstream &);
		std::string getName() const;
		void sendMessage(std::string);

	private:
		std::map<std::string, Channel *> _channels;
		Server &_server;
		int _clientFd;
		rank _rank;
		std::string _nickName;
};
