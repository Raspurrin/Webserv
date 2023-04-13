#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include <sys/socket.h>

void	error_handle(std::string type)
{
	perror(type.c_str());
	exit(0);
}

int main(void)
{
	//programming wtih tcp/ip sockets
	//1. create the socket using the system call socket(domain, type, protocol)
	//domain or address family: AF_INET macro for IP
	//type: SOCK_STREAM virtual circuit service TCP
	//protocol: 0 IP
	int	server_fd;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_handle("Couldnt create socket.");

	//2. name the socket
	//assigning a transport address to the socket, a port number. use bind() syscall.
	//filling sockaddr_in (structure describing an internet socket address) struct before bind()
	struct		sockaddr_in address;
	const int	port = 8080;

	address.sin_family = AF_INET;
	//INADDR_ANY 0.0.0.0 OS chooses
	//htonl converts the address to a network representation
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	//htons converts a short integer to a network representation
	address.sin_port = htons(port);

	//sockaddr (structure describing a generic socket address) struct before bind()
	//int bind(int socket, const struct sockaddr *address, socklen_t address_len)
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
		error_handle("Binding error");

	//3. listen /  wait for an incoming connection
	// listen (int socket, int backlog) puts the server socket in a passive mode
	// backlog defines the max # of pending connection that can queue b4 being refued with (ECONNREFUSED)
	if (listen(server_fd, 3) < 0)
		error_handle("listen");
	// accept
	int	new_socket;
	//4. send and receive messages
	//5. close the socket
	return (0);
}

