	#include "../header/ServerManager.class.hpp"
	#include "../header/ServerConfigParser.hpp"
	
	ServerManager::ServerManager(void) :
		_opt(1)
	{
		ServerConfigParser	parsingConfig("configuration.conf");
	
		_serverConfigs = parsingConfig.getServerConfigs();
		addServerSockets();
	}

	void	ServerManager::addServerSockets(void)
	{
		for (std::vector<ServerConfig>::iterator it = _serverConfigs.begin(); it != _serverConfigs.end(); it++)
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
		if (setsockopt(serverSocket.fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)))
			error_handle("setsockopt");
		if (bind(serverSocket.fd, reinterpret_cast<struct sockaddr *>(&serverConfig.getAddress()), sizeof(serverConfig.getAddress())) < 0)
			error_handle("Binding error");
		if (listen(serverSocket.fd, BACKLOG) < 0)
			error_handle("Listen error");
		_serverSockets.push_back(serverSocket);
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
			serverEvents = poll(_serverSockets.data(), _serverSockets.size(), 300);
			if (serverEvents > 0)
				listenToServerSockets();
			numEvent = poll(_clientSockets.data(), _clientSockets.size(), 300);
			if (numEvent > 0)
				handleClientSockets();
		}
	}

	void	ServerManager::listenToServerSockets()
	{
		for (size_t i = 0; i < _serverSockets.size(); i++)
		{
			if (_serverSockets[i].revents & POLLIN)
				addClientSocket(_serverSockets[i], _serverConfigs[i]);
		}
	}

	void	ServerManager::addClientSocket(t_pollfd &serverSocket, ServerConfig &serverConfig)
	{
		pollfd	newPfd;

		newPfd.fd = accept(serverSocket.fd, NULL, NULL);
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &_opt, sizeof(_opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		_clientSockets.push_back(newPfd);
		Client	newClient(newPfd, serverConfig);
		_clients.push_back(newClient);
	}

	void	ServerManager::sendResponse(Client &client, int indexToRemove)
	{
		std::cout << "==================" << std::endl;
		std::cout << "sending response" << std::endl;
		std::cout << "==================" << std::endl;
		send(client.getSocket(), client.getResponse().c_str(), client.getResponse().length(), 0);
		printf("HELLO MESSAGE SENT FROM SERVER\n");
		close(client.getSocket());
		(void)indexToRemove;
	//	_clientSockets.erase(_clientSockets.begin() + indexToRemove);
	//	_clients.erase(_clients.begin() + indexToRemove);
	}

void	ServerManager::handleClientSockets()
{

		for (size_t i = 0, size = _clients.size(); i < size; i++)
		{
			//std::cout << "we do a loop" << std::endl;
			if (_clientSockets[i].revents & POLLIN)
			{
				std::cout << "POLLIN" << std::endl;
				_clients[i].getRequest();
			}
			else if (_clients[i].isRequestSent() && _clientSockets[i].revents & POLLOUT)
				sendResponse(_clients[i], i);
			else if (_clientSockets[i].revents & POLLERR)
				error_handle("Error occurred with a connection");
		}
	}

	ServerManager::~ServerManager(void)
	{
	}
