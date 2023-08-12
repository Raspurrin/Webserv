#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerManager.class.hpp"
#include <sys/socket.h>
#include <fcntl.h>

volatile sig_atomic_t	g_shutdown_flag = 0;

void signalHandler(int sigNum)
{
	if (sigNum == SIGINT || sigNum == SIGTERM)
	{
		std::cout << "Received shutdown signal. Terminating processes..." << std::endl;
		g_shutdown_flag = 1;
	}
}

int main(int argc, char** argv)
{
	std::string configuration_file = "configuration.conf";

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	if (argc == 2)
		configuration_file = argv[1];
	else if (argc != 1)
	{
		std::cout << "Wrong arguments." << std::endl;
		exit(1);
	}
	ServerManager ServerManager(configuration_file.c_str());
	ServerManager.eventLoop();
	return (0);
}
