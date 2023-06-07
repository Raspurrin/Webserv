#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerManager.class.hpp"
#include <sys/socket.h>
#include <fcntl.h>

int main(void)
{
	// ServerManager ServerManager;

	int fd = open("example_request.txt", O_RDONLY);
	Request request;
	request.getRequest(fd);
	std::cout << request["Content-Type"] << std::endl;
	request.printBody();
	//ServerManager.eventLoop();
	return (0);
}
