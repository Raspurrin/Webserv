#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerManager.class.hpp"
#include <sys/socket.h>
#include <fcntl.h>

int main(void)
{
	// ServerManager ServerManager;

	// ServerManager.eventLoop();
	ServerConfigParser parser("configuration.conf");
	ServerConfigVector serverConfigs = parser.getServerConfigs();
	std::cout << RED << "\nOutside Parser:" << DEF << std::endl;
	for (const auto &it : serverConfigs)
		it.printServerConfig();
	return (0);
}
