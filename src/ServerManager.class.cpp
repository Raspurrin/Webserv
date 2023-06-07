	#include "../header/ServerManager.class.hpp"
	
	ServerManager::ServerManager(void) :
		opt(1)
	{
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(8080);
		addressLen = sizeof(address);
	}

	void	ServerManager::parseConfigFile(void)
	{

	}

	void	ServerManager::configureSocket(int newSocket)
	{
		int		flags;

		if (setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		flags = fcntl(newSocket, F_GETFL, 0);
		fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
	}

	void	ServerManager::addServerSocket(void)
	{
		pollfd	serverSocket;
	
		serverSocket.events = POLLIN;
		if ((serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket.fd);
		if (bind(serverSocket.fd, (struct sockaddr *) &address, sizeof(address)) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.fd, 3) < 0)
			error_handle("Listen error");
		sockets.push_back(serverSocket);
		serverSockets.push_back(serverSocket);
	}

	void	ServerManager::addClientSocket(int serverSocket)
	{
		pollfd	newPfd;

		newPfd.fd = accept(serverSocket, (struct sockaddr *) &address, (socklen_t *) &addressLen);
		// if (newPfd.fd > 0)
		// 	addNewConnection(newPfd.fd);
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		sockets.push_back(newPfd);
	}

	// void	ServerManager::getRequest(int new_socket)
	// {
	// 	char	buffer[30000];

	// 	read(new_socket, buffer, 30000);
	// 	Request	request(buffer);
	// 	request.printMap();
	// 	response = request.getResponse();	
	// }

	// void	ServerManager::postResponse(int socket, int indexToRemove)
	// {
	// 	send(socket, response.c_str(), response.length(), 0);
	// 	printf("HELLO MESSAGE SENT FROM ServerManager\n");
	// 	sockets.erase(sockets.begin() + indexToRemove);
	// 	close(socket);
	// }

	void	ServerManager::eventLoop(void)
	{
		int	newSocket;
		int	numEvent;

		while (69)
		{
			int	i = 0;
			numEvent = poll(sockets.data(), sockets.size(), 300);
			if (numEvent > 0)
			{
				while (i < serverSockets.size())
				{
					if (serverSockets[i].revents & POLLIN)
						addClientSocket(serverSockets[i].fd);
					i++;
				}
				while (i < clients.size() - serverSockets.size())
				{
					if (sockets[i].revents & POLLIN)
						getRequest(clients[i], sockets[i]->ServerConfig);
					else if (sockets[i].revents & POLLOUT)
						postResponse(sockets[i].fd, i);
					else if (sockets[i].revents & POLLERR)
						error_handle("Error occured with a connection");
					i++;
				}
			}
		}
	}

	// Socket::Socket(Socket const &src)
	// {

	// }

	// ServerManager&	ServerManager::operator=(ServerManager const &assign)
	// {

	// }

	ServerManager::~ServerManager(void)
	{
		// for (size_t i = 0; i < pollFds.size(); i++)
		// {
		// 	close(pollFds[i].fd);
		// }
	}
