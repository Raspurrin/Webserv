#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerManager.class.hpp"
#include <sys/socket.h>
#include <fcntl.h>

void signalHandler(int sigNum)
{
	std::cout
		<< RED << "\nInterrupt signal caught.\n"
		<< CYAN << "Shutting down web server...Bye!"
		<< DEF << std::endl;
	exit(sigNum);
}

int main(void)
{
	signal(SIGINT, signalHandler);
	ServerManager ServerManager;

	ServerManager.eventLoop();
	return (0);
}
