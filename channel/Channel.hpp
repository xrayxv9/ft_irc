#pragma once

#include <iostream>
#include <vector>

class Client;

class Channel
{
	public :
		Channel(std::string);
		Channel(std::string, Client *);
		~Channel();
		void sendMessage(const std::string &);
		void recv();
		std::vector<Client *> &getClients();
		std::vector<Client *> &getModo();
		std::string getName();	
		void kick(Client *);
		template< class T>
		void kick(T);
		void setInviteOnly( bool inviteOnly );
		bool isInviteOnly();
		std::string &getMode();
		

	private :
		bool _inviteOnly;
		std::string _mode;
		std::string _channelName;
		std::vector<Client *> _clientList;
		std::vector<Client *> _modoList;
};

#include "Channel.tpp"
