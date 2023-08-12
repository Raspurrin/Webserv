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

int main(void)
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	ServerManager ServerManager;

	ServerManager.eventLoop();
	return (0);
}
