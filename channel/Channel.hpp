#pragma once

#include <iostream>
#include <iterator>
#include <vector>
#include <ctime>

class Client;

class Channel
{
	public :
		Channel(std::string, std::string);
		Channel(std::string, std::string, Client *);
		~Channel();
		void sendMessage(const std::string &);
		void recv();
		std::string getName();	
		void kick(Client *);
		template< class T>
		void kick(T);
		// setter
	
		void setInviteOnly( bool, Client * );
		void restrictTopic( bool, Client * );
		void setPassword( std::string, bool, Client * );
		void setModo( Client *, bool, Client * );
		void setUserLimit( bool, int, Client * );
		void setMode( char, bool, Client *, std::string );
		void setTimeStamp();

		// getter
		bool isInviteOnly();
		bool isTopicRestricted();
		std::string	getPassword();
		std::string &getMode();
		std::vector<Client *> &getClients();
		std::vector<Client *> &getModo();
		std::vector<Client *> &getInvited();
		int getUserLimit();
		std::vector<Client *>::iterator isModo( Client * );
		time_t getTimeStamp();


		const std::string &getTopic() const;
		void setTopic(const std::string &topic, const std::string &who);
		const std::string &whoSetTopic() const;
		const time_t &lastUpate() const;
		const std::string &whoSetTopicMask() const;
		void setTopicMask(const std::string &);
		const std::string &getPassword() const;
		void removeClient(Client *);
		
	private :
		bool _inviteOnly;
		int _userLimit;
		bool _isTopicRestricted;
		bool _isPasswordLimited;
		std::string _mode;
		std::string _channelName;
		std::string _password;
		std::vector<Client *> _clientList;
		std::vector<Client *> _modoList;
		std::vector<Client *> _invitedList;
		std::string _topic;
		std::string _whoSetTopic;
		std::string _whoSetTopicMask;
		std::time_t _ts;
		std::time_t _tsMode;
};

#include "Channel.tpp"
