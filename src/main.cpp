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

	// ServerManager.eventLoop();
	ServerConfigParser parser("configuration.conf");
	ServerConfigVector serverConfigs = parser.getServerConfigs();
	std::cout << RED << "\nOutside Parser:" << DEF << std::endl;
	for (const auto &it : serverConfigs)
		it.printServerConfig();
	return (0);
}
