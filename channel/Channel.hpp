#pragma once

#include <iostream>
#include <iterator>
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
		

		// getter
		bool isInviteOnly();
		bool isRestricted();
		std::string	getPassword();
		std::string &getMode();
		std::vector<Client *> &getClients();
		std::vector<Client *> &getModo();
		int getUserLimit();
		std::vector<Client *>::iterator isModo( Client * );

	private :
		bool _inviteOnly;
		int _userLimit;
		bool _isAdminRestricted;
		bool _isPasswordLimited;
		std::string _mode;
		std::string _channelName;
		std::string _password;
		std::vector<Client *> _clientList;
		std::vector<Client *> _modoList;
};

#include "Channel.tpp"
