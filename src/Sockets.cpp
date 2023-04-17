	#include "Sockets.hpp"
	
	Sockets::Sockets(void) :
		port(8080),
		opt(1),
		addressLen(sizeof(address))
	{
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
		if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		if (bind(serverFd, (struct sockaddr *) &address, sizeof(address)) < 0)
			error_handle("Binding error");
		if (listen(serverFd, 3) < 0)
			error_handle("Listen error");
	}

	void	Sockets::addNewConnection(int fd)
	{
		pollfd	newPfd;
		int		flags = fcntl(fd, F_GETFL, 0);

		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
		newPfd.fd = fd;
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		lol.push_back(newPfd);
	}

	void	Sockets::readMessage(int new_socket)
	{
		char	buffer[30000];

		read(new_socket, buffer, 30000);
		printf("in buffer: %s\n", buffer);
		Request	request(buffer);
		request.printMap();
		//std::cout << readed << std::endl;
	}

	void	Sockets::sendMessage(int new_socket)
	{
		std::string	hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 17\n\nHello from Server";

		send(new_socket, hello.c_str(), hello.length(), 0);
		printf("HELLO MESSAGE SENT FROM SERVER\n");
	}

	void	Sockets::checkConnections(void)
	{
		int	newSocket;
		int	numEvent;

		while (69)
		{
			printf("...WAITING FOR NEW CONNECTION...\n");
			if ((newSocket = accept(serverFd, (struct sockaddr *) &address, (socklen_t *) &addressLen)) < 0)
				error_handle("Accept error");
			addNewConnection(newSocket);
			numEvent = poll(lol.data(), lol.size(), 0);
			if (numEvent > 0)
			{
				for (size_t i = 0; i < lol.size(); i++)
				{
					if (lol[i].revents & POLLIN)
						readMessage(lol[i].fd);
					else if (lol[i].revents & POLLOUT)
						sendMessage(lol[i].fd);
					else if (lol[i].revents & POLLERR)
						error_handle("Error occured with a connection");
				}
			}
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
		for (size_t i = 0; i < lol.size(); i++)
		{
			close(lol[i].fd);
		}
	}
