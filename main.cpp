#include <iostream>


int main(int argc, char **argv)
{
	std::string portPreConvert;
	std::string passwd;
	int port;
	
	if (argc != 3)
	{
		std::cerr << "Please give at least a port and a password" << std::endl;
		std::cout << "./ft_irc <port> <password>" << std::endl;
		return 1;
	}
	portPreConvert = argv[1];
	if (portPreConvert.length() > 5)
	{
		std::cerr << "The port you have given is incorrect" << std::endl;
		return 1;
	}
	port = atoi(portPreConvert.c_str());
	if (port > 65535)
	{
		std::cerr << "The port you have given is incorrect" << std::endl;
		return 1;
	}
}
