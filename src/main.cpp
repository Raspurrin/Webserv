#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/socket.hpp"
#include <sys/socket.h>

int main(void)
{
	Sockets sockets;

	sockets.checkConnections();
	return (0);
}

