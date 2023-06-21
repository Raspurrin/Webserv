	#include "../header/ServerManager.class.hpp"
	#include "../header/ParsingConfig.hpp"
	
	ServerManager::ServerManager(void) :
		opt(1)
	{
		ParsingConfig	parsingConfig;
		int fd = open("serverconfig", O_RDONLY);
	
		serverConfigs = parsingConfig.parsing(fd); // TODO: serverconfigs type needs to change in the serverconfigs class
		// open file in parsingConfig
		addServerSockets();
	}

	void	ServerManager::addServerSockets(void)
	{
		for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); it++)
			addServerSocket(*it);
	}

	void	ServerManager::addServerSocket(ServerConfig &serverConfig)
	{ 
		t_serverSocket serverSocket;
	
		//TODO: initialize sockadrr in server vector
		serverSocket.address.sin_family = AF_INET;
   		serverSocket.address.sin_addr.s_addr = INADDR_ANY;
    	serverSocket.address.sin_port = htons(serverConfig.getPort());

		serverSocket.poll.events = POLLIN;
		if ((serverSocket.poll.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket.poll.fd);
		if (bind(serverSocket.poll.fd, (struct sockaddr *) &serverSocket.address, sizeof(serverSocket.address)) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.poll.fd, BACKLOG) < 0)
			error_handle("Listen error");
		serverSockets.push_back(serverSocket);
		// where to add the socket to? 
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
			numEvent = poll(clientSockets.data(), clientSockets.size(), 300);
			if (numEvent > 0)
			{
				listenToServerSockets();
				handleClientSockets();
			}
		}
	}

	void	ServerManager::listenToServerSockets()
	{
		for (size_t i = 0; i < serverSockets.size(); i++)
		{
			if (serverSockets[i].poll.revents & POLLIN) // should we have a vector of pairs here to pass the address?
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
		clientSockets.push_back(newPfd);
		//TODO: instantiate client class and add to vector
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
