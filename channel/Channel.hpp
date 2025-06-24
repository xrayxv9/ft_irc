#pragma once

#include <iostream>
#include <vector>
#include <ctime>

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
		void setTopic(const std::string &topic, const std::string &who);
		const std::string &whoSetTopic() const;
		const time_t &lastUpate() const;
	private :
		std::string _channelName;
		std::vector<Client *> _clientList;
		std::string _topic;
		std::string _whoSetTopic;
		std::time_t _ts;
};

#include "Channel.tpp"
