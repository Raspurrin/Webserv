#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerManager.class.hpp"
#include <sys/socket.h>
#include <fcntl.h>

int main(void)
{
	ServerManager ServerManager;

	ServerManager.eventLoop();
	return (0);
}
