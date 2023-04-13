#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include <sys/socket.h>

int main(void)
{
	//programming wtih tcp/ip sockets
	//1. create the socket using the system call socket(domain, type, protocol)
	//domain: AF_INET macro for IP
	//type: SOCK_STREAM virtual circuit service TCP
	//protocol: 0 IP
	int	server_fd;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Couldnt create socket.");
		exit(0);
	}
	//2. identify the socket
	//3. wait for an incoming connection
	//4. send and receive messages
	//5. close the socket
	return (0);
}

