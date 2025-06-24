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
		const std::string &getTopic() const;
		void setTopic(const std::string &topic);

	private :
		std::string _channelName;
		std::vector<Client *> _clientList;
		std::string _topic;
};

#include "Channel.tpp"
