	#include "../header/ServerManager.class.hpp"
	#include "../header/ParsingConfig.hpp"
	
	ServerManager::ServerManager(void) :
		opt(1)
	{
		ParsingConfig	parsingConfig;
	
		serverConfigs = parsingConfig.parsing("config");
		addServerSockets();
	}

	void	ServerManager::addServerSockets(void)
	{
		std::cout << "adding server sockets" << std::endl;
		for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); it++)
			addServerSocket(*it);
	}

	void	ServerManager::addServerSocket(ServerConfig &serverConfig)
	{ 
		t_pollfd serverSocket;
	
		if ((serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket.fd);
		if (bind(serverSocket.fd, reinterpret_cast<struct sockaddr *>(&serverConfig.getAddress()), sizeof(serverConfig.getAddress())) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.fd, BACKLOG) < 0)
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
		int serverEvents;
		std::cout << "starting event loop" << std::endl;

		while (69)
		{
			listenToServerSockets();
			serverEvents = poll(serverSockets.data(), serverSockets.size(), 300);
			std::cout << serverEvents << " server events polled" << std::endl;
			numEvent = poll(clientSockets.data(), clientSockets.size(), 300);
			for (size_t i = 0; i < serverSockets.size(); i++)
			{
				if (serverSockets[i].revents & POLLIN)
					addClientSocket(serverSockets[i], serverConfigs[i]);
			}
			std::cout << numEvent << " events polled" << std::endl;
			if (numEvent > 0)
			{
				handleClientSockets();
			}
		}
	}

	void	ServerManager::listenToServerSockets()
	{
		std::cout << "listening to server sockets. serverSockets.size() = " << serverSockets.size() << std::endl;
		for (size_t i = 0; i < serverSockets.size(); i++)
		{
			if (serverSockets[i].revents & POLLIN)
				addClientSocket(serverSockets[i], serverConfigs[i]);
		}
	}

	void	ServerManager::addClientSocket(t_pollfd &serverSocket, ServerConfig &serverConfig)
	{
		std::cout << "client connect" << std::endl;
		pollfd	newPfd;
		Client	newClient(newPfd, serverConfig);
		newPfd.fd = accept(serverSocket.fd, reinterpret_cast<struct sockaddr *>(&serverConfig.getAddress()), \
											(socklen_t *) sizeof(serverConfig.getAddress()));
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		clientSockets.push_back(newPfd);
		clients.push_back(newClient);
	}

	//TODO: remove client class and client socket from vector when disconnecting

	void	ServerManager::handleClientSockets()
	{
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (clientSockets[i].revents & POLLIN)
				clients[i].getRequest();
			else if (clientSockets[i].revents & POLLOUT)
				clients[i].getResponse();
			else if (clientSockets[i].revents & POLLERR)
				error_handle("Error occurred with a connection");
		}
	}

	ServerManager::~ServerManager(void)
	{
	}
