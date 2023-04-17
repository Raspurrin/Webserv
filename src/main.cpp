#include "../header/Webserv.hpp"
#include "../header/colours.hpp"
#include "../header/Sockets.hpp"
#include <sys/socket.h>

int main(void)
{
	Sockets sockets;

	sockets.checkConnections();
	return (0);
}

