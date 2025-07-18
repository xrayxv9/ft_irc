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
class Channel;

class Client
{
	public:
		Client( int fd, Server &);
		~Client();
		Server &getServer();
		int getFd() const;
		std::map<std::string, Channel *> &getChannels();
		void joinChannel(const std::string &, const std::string &);
		void sendMessage(std::ostringstream &);
		std::string getUserName() const;
		std::string getNickName() const;
		void sendMessage(std::string);
		int	isRegistered() const;
		void logIn();
		std::string generateMask();
		void setNickName( std::string );
		void setUserName( std::string );
		std::vector<std::string> &getQueue();
		int updateQueue();
		bool isMod(Channel *) const;
		bool isInChannel(Channel *) const;
		void sendUsersList(Channel *);

	private:
		std::map<std::string, Channel *> _channels;
		Server &_server;
		int _clientFd;
		rank _rank;
		std::string _nickName;
		int _isRegistered;
		std::string _userName;
		std::vector<std::string> _queue;
		std::string _buff;
};
