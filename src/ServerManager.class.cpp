	#include "../header/ServerManager.class.hpp"
	#include "../header/ParsingConfig.hpp"
	
	ServerManager::ServerManager(void) :
		opt(1)
	{
		ParsingConfig	parsingConfig;
		int fd = open("serverconfig", O_RDONLY);
	
		serverConfigs = parsingConfig.parsing(fd);
		addServerSockets();
	}

	void	ServerManager::addServerSockets(void)
	{
		for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); it++)
			addServerSocket(it->getServerAddress());
	}

	void	ServerManager::addServerSocket(struct sockaddr_in serverAddress)
	{ 
		pollfd	serverSocket;
	
		serverSocket.events = POLLIN;
		if ((serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket.fd);
		if (bind(serverSocket.fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.fd, 3) < 0)
			error_handle("Listen error");
		serverSockets.push_back(serverSocket);
	}

	void	ServerManager::configureSocket(int newSocket)
	{
		int		flags;

		if (setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		flags = fcntl(newSocket, F_GETFL, 0);
		fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
	}

	void	ServerManager::eventLoop(void)
	{
		int	numEvent;

		while (69)
		{
			numEvent = poll(sockets.data(), sockets.size(), 300);
			if (numEvent > 0)
			{
				listenToServerSockets();
				handleClientSockets();
			}
		}
	}

	void	ServerManager::listenToServerSockets()
	{
		for (size_t i = 0; i < serverConfigs.size(); i++)
		{
			if (serverSockets[i].revents & POLLIN)
				addClientSocket(serverConfigs[i]);
		}
	}

	void	ServerManager::addClientSocket(ServerConfig& serverConfig)
	{
		pollfd	newPfd;

		newPfd.fd = accept(serverConfig.getServerSocketFd(), &serverConfig.getServerAddress(), \
															(socklen_t *) sizeof(serverConfig.getServerAddress()));
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		sockets.push_back(newPfd);
	}

	void	ServerManager::handleClientSockets()
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (sockets[i].revents & POLLIN)
				clients[i].getRequest();
			else if (sockets[i].revents & POLLOUT)
				clients[i].getResponse();
			else if (sockets[i].revents & POLLERR)
				error_handle("Error occurred with a connection");
		}
	}

	ServerManager::~ServerManager(void)
	{
	}
