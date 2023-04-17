	#include "Sockets.hpp"
	
	Sockets::Sockets(void) :
		port(8080),
		opt(1),
		address_len(sizeof(address))
	{
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
		if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
			error_handle("Binding error");
		if (listen(server_fd, 3) < 0)
			error_handle("Listen error");
	}

	void	Sockets::checkConnections(void)
	{
		while (1)
		{
			int	new_socket;
			std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 17\n\nHello from Server";

			printf("...WAITING FOR NEW CONNECTION...\n");
			if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address_len)) < 0)
				error_handle("Accept error");
			readed = read(new_socket, buffer, 30000);
			printf("in buffer: %s\n", buffer);
			Request	request(buffer);
			std::cout << readed << std::endl;
			send(new_socket, hello.c_str(), hello.length(), 0);
			printf("HELLO MESSAGE SENT FROM SERVER\n");
			close(new_socket);
		}
	}

	// Socket::Socket(Socket const &src)
	// {

	// }

	// Sockets&	Sockets::operator=(Sockets const &assign)
	// {

	// }

	Sockets::~Sockets(void)
	{

	}
