#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

int main(void)
{
	//programming wtih tcp/ip sockets
	//1. create the socket using the system call socket(domain, type, protocol)
	//domain or address family: AF_INET macro for IP
	//type: SOCK_STREAM virtual circuit service TCP
	//protocol: 0 IP
	int	server_fd;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Couldnt create socket.");
		exit(0);
	}
	//2. name the socket
	//assigning a transport address to the socket, a port number. use bind() syscall.
	//int bind(int socket, const struct sockaddr *address, socklen_t address_len)
	//filling sockaddr struct before bind()
	struct		sockaddr_in address;
	const int	port = 8080;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //0.0.0.0 OS chooses
	address.sin_port = htons(port); //htons converts a short integer to a network representation
	
	//
	//3. wait for an incoming connection
	//4. send and receive messages
	//5. close the socket
	return (0);
}

