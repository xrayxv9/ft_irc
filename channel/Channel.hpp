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
		std::string getName();	
		void kick(Client *);
		template< class T>
		void kick(T);

	private :
		std::string _channelName;
		std::vector<Client *> _clientList;
};

#include "Channel.tpp"
