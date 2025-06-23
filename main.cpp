
#include <Server.hpp>

bool g_running = true;


void handle_sigint( int sig )
{
	(void)sig;
	g_running = false;
}

int main(int argc, char **argv)
{
	std::string portPreConvert;
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
	port = std::atoi(portPreConvert.c_str());
	if (port > 65535)
	{
		std::cerr << "The port you have given is incorrect" << std::endl;
		return 1;
	}
	std::signal(SIGINT, handle_sigint);
	Server *serv = new Server(port, argv[2]);
	if (serv->getStatus() == 0)
	{
		delete serv;
		return 1;
	}
	serv->run();
	delete serv;
}
