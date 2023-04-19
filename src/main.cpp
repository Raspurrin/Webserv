#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/Server.class.hpp"
#include <sys/socket.h>

int main(void)
{
	Server Server;

	Server.checkConnections();
	return (0);
}

