#pragma once

#include <iostream>
#include <vector>

class Client;

class Channel
{
	public :
		Channel();
		void send();
		void recv();
	private :
		std::vector<Client> _clientList;
};
