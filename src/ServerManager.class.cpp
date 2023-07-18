	#include "../header/ServerManager.class.hpp"
	#include "../header/ParsingConfig.hpp"
	#include "../header/Cgi.class.hpp"

IntVector	_indexesToRemove;

	ServerManager::ServerManager(void) :
		_opt(1), _numServerSockets(0)
	{
		ParsingConfig	parsingConfig;

		_serverConfigs = parsingConfig.parsing("config");
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
		_sockets.push_back(serverSocket);
		++_numServerSockets;
	}

	void	ServerManager::configureSocket(int newSocket)
	{
		int		flags;
		if (DEBUG)
			std::cout << "socket2 " << newSocket << std::endl;
		flags = fcntl(newSocket, F_GETFL, 0);
		fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
	}

	void	ServerManager::eventLoop(void)
	{
		int	numEvent;

		while (69)
		{
			// no timeout needed since poll checks now both, servers and connections
			numEvent = poll(_sockets.data(), _sockets.size(), -1);
			if (numEvent > 0) {
				for (size_t i = 0; i < _sockets.size(); ++i) {
					if (i < (size_t)_numServerSockets)
						listenToServerSocket(i);
					else {
						handleClientSocket(i);
					}
					removeIndexes();
				}
			}
		}
	}

	void	ServerManager::listenToServerSocket(int i)
	{
			if (_sockets[i].revents & POLLIN)
				addClientSocket(_sockets[i], _serverConfigs[i]);
	}

	void	ServerManager::addClientSocket(t_pollfd &serverSocket, ServerConfig &serverConfig)
	{
		pollfd	newPfd;

		newPfd.fd = accept(serverSocket.fd, NULL, NULL);
		configureSocket(newPfd.fd);
		setsockopt(newPfd.fd, IPPROTO_TCP, TCP_NODELAY, &_opt, sizeof(_opt));
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		_sockets.push_back(newPfd);
		Client	newClient(newPfd, serverConfig);
		_clients.push_back(newClient);
	}

	void	ServerManager::sendResponse(Client &client)
	{
		std::string	_response = client.getResponse();
		static int i = 0;

		if (DEBUG)
		{
			std::cout << "==================" << std::endl;
			std::cout << "sending response " << i << std::endl;
			std::cout << "==================" << std::endl;
		}
		send(client.getSocket(),_response.c_str(), _response.length(), 0);
		if (DEBUG)
			printf("MESSAGE SENT FROM SERVER\n");
		close(client.getSocket());
		i++;
	}

	void	ServerManager::handleClientSocket(int i)
	{
		// std::cout << "we do a loop" << std::endl;
		if (_sockets[i].revents & POLLIN)
		{
			if (DEBUG)
				std::cout << "POLLIN with index " << i << "fd is " << _sockets[i].fd << std::endl;
			_clients[i - _numServerSockets].getRequest();
		}
		else if (_clients[i - _numServerSockets].isRequestSent() && _sockets[i].revents & POLLOUT) {
			sendResponse(_clients[i - _numServerSockets]);
			_indexesToRemove.push_back(i);
		}
		else if (_sockets[i].revents & POLLERR)
			error_handle("Error occurred with a connection");
	}

	void	ServerManager::removeIndexes()
	{
		for (size_t i = 0; i < _indexesToRemove.size(); ++i) {
			_sockets.erase(_sockets.begin() + _indexesToRemove[i]);
			_clients.erase(_clients.begin() + (_indexesToRemove[i] - _numServerSockets));
		}
		_indexesToRemove.clear();
	}

	ServerManager::~ServerManager(void)
	{
	}
