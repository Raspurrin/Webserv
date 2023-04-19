	#include "../header/Sockets.class.hpp"
	
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
		pollFds.push_back(newPfd);
	}

	void	Sockets::readMessage(int new_socket)
	{
		char	buffer[30000];

		read(new_socket, buffer, 30000);
		printf("---------Buffer in sockets is:\n %s\n--- BUFFER DONE\n", buffer);
		Request	request(buffer);
		request.printMap();
		response = request.getResponse();	
		std::cout << "RESPONSE IS: " << response << std::endl;
	}

	void	Sockets::sendMessage(int new_socket)
	{
		send(new_socket, response.c_str(), response.length(), 0);
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
			numEvent = poll(pollFds.data(), pollFds.size(), 0);
			if (numEvent > 0)
			{
				for (size_t i = 0; i < pollFds.size(); i++)
				{
					if (pollFds[i].revents & POLLIN)
						readMessage(pollFds[i].fd);
					else if (pollFds[i].revents & POLLOUT)
						sendMessage(pollFds[i].fd);
					else if (pollFds[i].revents & POLLERR)
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
		for (size_t i = 0; i < pollFds.size(); i++)
		{
			close(pollFds[i].fd);
		}
	}
