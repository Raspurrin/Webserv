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
		for (std::vector<ServerConfig>::iterator it = serverConfigs.begin(); it != serverConfigs.end(); it++)
			addServerSocket(*it);
	}

	void	ServerManager::addServerSocket(ServerConfig &serverConfig)
	{ 
		t_pollfd serverSocket;
	
		serverSocket.events = POLLIN;
		if ((serverSocket.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		std::cout << "socket " << serverSocket.fd << std::endl;
		configureSocket(serverSocket.fd);
		if (setsockopt(serverSocket.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		if (bind(serverSocket.fd, reinterpret_cast<struct sockaddr *>(&serverConfig.getAddress()), sizeof(serverConfig.getAddress())) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.fd, BACKLOG) < 0)
			error_handle("Listen error");
		serverSockets.push_back(serverSocket);
	}

	void	ServerManager::configureSocket(int newSocket)
	{
		int		flags;
		std::cout << "socket2 " << newSocket << std::endl;
		flags = fcntl(newSocket, F_GETFL, 0);
		fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
	}

	void	ServerManager::eventLoop(void)
	{
		int	numEvent;
		int serverEvents;

		while (69)
		{
			serverEvents = poll(serverSockets.data(), serverSockets.size(), 300);
			if (serverEvents > 0)
				listenToServerSockets();
			numEvent = poll(clientSockets.data(), clientSockets.size(), 300);
			if (numEvent > 0)
				handleClientSockets();
		}
	}

	void	ServerManager::listenToServerSockets()
	{
		for (size_t i = 0; i < serverSockets.size(); i++)
		{
			if (serverSockets[i].revents & POLLIN)
				addClientSocket(serverSockets[i], serverConfigs[i]);
		}
	}

	void	ServerManager::addClientSocket(t_pollfd &serverSocket, ServerConfig &serverConfig)
	{
		pollfd	newPfd;

		newPfd.fd = accept(serverSocket.fd, NULL, NULL);
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		clientSockets.push_back(newPfd);
		Client	newClient(newPfd, serverConfig);
		clients.push_back(newClient);
	}

	void	ServerManager::sendResponse(Client &client, int indexToRemove)
	{
		std::cout << "==================" << std::endl;
		std::cout << "sending response" << std::endl;
		std::cout << "==================" << std::endl;
		send(client.getSocket(), client.getResponse().c_str(), client.getResponse().length(), 0);
		printf("HELLO MESSAGE SENT FROM SERVER\n");
		close(client.getSocket());
		clientSockets.erase(clientSockets.begin() + indexToRemove);
		clients.erase(clients.begin() + indexToRemove);
	}

	//TODO: remove client class and client socket from vector when disconnecting

	void	ServerManager::handleClientSockets()
	{

		for (size_t i = 0, size = clients.size(); i < size; i++)
		{
			//std::cout << "we do a loop" << std::endl;
			if (clientSockets[i].revents & POLLIN)
			{
				std::cout << "POLLIN" << std::endl;
				clients[i].getRequest();
			}
			else if (clients[i].isRequestSent() && clientSockets[i].revents & POLLOUT)
				sendResponse(clients[i], i);
			else if (clientSockets[i].revents & POLLERR)
				error_handle("Error occurred with a connection");
		}
	}

	ServerManager::~ServerManager(void)
	{
	}
