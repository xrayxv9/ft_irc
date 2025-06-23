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

	private :
		std::string _channelName;
		std::vector<Client *> _clientList;
};
