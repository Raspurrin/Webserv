	#include "../header/ServerManager.class.hpp"
	#include "../header/ServerConfigParser.class.hpp"
	#include "../header/Cgi.class.hpp"

	IntVector	_indexesToRemove;

	ServerManager::ServerManager(void) :
		_opt(1), _numServerSockets(0)
	{
		ServerConfigParser	parsingConfig("configuration.conf");

		_serverConfigs = parsingConfig.getServerConfigs();
		addServerSockets();
	}

	void	ServerManager::addServerSockets(void)
	{
		for (std::vector<ServerConfig>::iterator it = _serverConfigs.begin(); it != _serverConfigs.end(); it++)
		{
			if (DEBUG)
				it->printServerConfig();
			addServerSocket(*it);
		}
	}

	void	ServerManager::addServerSocket(ServerConfig &serverConfig)
	{
		int	listen_socket_fd;

		if ((listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error in webserver");

		if (DEBUG)
			std::cout << "- addServerSocket: Socket " << listen_socket_fd << std::endl;

		configureSocket(listen_socket_fd);

		if (setsockopt(listen_socket_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt)))
		{
			close(listen_socket_fd);
			error_handle("setsockopt");
		}

		if (bind(listen_socket_fd, reinterpret_cast<struct sockaddr *>(&serverConfig.getAddress()), sizeof(serverConfig.getAddress())) < 0)
		{
			close(listen_socket_fd);
			error_handle("Binding error");
		}

		if (listen(listen_socket_fd, BACKLOG) < 0)
		{
			close(listen_socket_fd);
			error_handle("Listen error");
		}

		t_pollfd serverSocket;
		serverSocket.fd = listen_socket_fd;
		serverSocket.events = POLLIN;

		_sockets.push_back(serverSocket);
		++_numServerSockets;
	}

	void	ServerManager::configureSocket(int newSocket)
	{
		if (DEBUG)
			std::cout << "- configureSocket: Socket " << newSocket << std::endl;

		int		flags;

		flags = fcntl(newSocket, F_GETFL, 0);
		if (flags == -1)
			error_handle("Failed to get socket flags in fcntl().");
		if (fcntl(newSocket, F_SETFL, flags | O_NONBLOCK) == -1)
			error_handle("Failed to set socket to non-blocking in fcntl().");
	}

	void	ServerManager::eventLoop(void)
	{
		int	numEvent;

		while (!g_shutdown_flag)
		{
			numEvent = poll(_sockets.data(), _sockets.size(), -1);
			if (numEvent == -1)
			{
				perror("Poll error");
				break ;
			}
			else if (numEvent == 0)
				continue ;
			for (size_t i = 0; i < _sockets.size(); ++i)
			{
				if (i < (size_t)_numServerSockets)
				{
					listenToServerSocket(i);
					if (g_shutdown_flag)
						break ;
				}
				else {
					handleClientSocket(i);
				}
				removeIndexes();
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
		int	newClientSocket;

		newClientSocket = accept(serverSocket.fd, NULL, NULL);
		if (newClientSocket == -1)
			perror("Accept error");
		else
		{
			configureSocket(newClientSocket);
			setsockopt(newClientSocket, IPPROTO_TCP, TCP_NODELAY, &_opt, sizeof(_opt));

			pollfd	newPfd;
			newPfd.fd = newClientSocket;
			newPfd.events = POLLIN | POLLOUT | POLLERR;
			newPfd.revents = 0;
			_sockets.push_back(newPfd);
			Client	newClient(newPfd, serverConfig);
			_clients.push_back(newClient);
		}
	}

	void	ServerManager::sendResponse(Client &client)
	{
		std::string	_response = client.getResponse();
		static int i = 0;

		if (DEBUG)
		{
			std::cout
				<< YELLOW << "========= "
				<< DEF << "Sending response: " << i
				<< YELLOW << " =========\n\n" << DEF;
		}
		int err = send(client.getSocket(),_response.c_str(), _response.length(), 0);
		if (DEBUG)
			std::cout << RED << "------> SUCCESS: Message sent from server. <-------\n\n" << DEF;
		if (client.responseFinished() || err == -1 || (size_t)err != _response.length()) {
			close(client.getSocket());
		}
		i++;
	}

	void	ServerManager::handleClientSocket(int i)
	{
		if (!_clients[i - _numServerSockets].isRequestSent() && time(NULL) - _clients[i - _numServerSockets].getLastActivity() > REQUEST_TIMEOUT) {
			_clients[i - _numServerSockets].setRequestError(ErrorResponse(408, "from ServerManager"));
		}
		if (_sockets[i].revents & POLLERR || _sockets[i].revents & POLLHUP || _sockets[i].revents & POLLPRI || _sockets[i].revents & POLLNVAL) {
			_indexesToRemove.push_back(i);
		}
		else if (_sockets[i].revents & POLLIN && !_clients[i - _numServerSockets].isRequestSent())
		{
			if (DEBUG)
				std::cout << "- POLLIN with index " << i << " fd is " << _sockets[i].fd << std::endl;
			_clients[i - _numServerSockets].getRequest();
		}
		else if (_clients[i - _numServerSockets].isRequestSent() && _sockets[i].revents & POLLOUT) {
			sendResponse(_clients[i - _numServerSockets]);
			if (_clients[i - _numServerSockets].responseFinished()) {
				_indexesToRemove.push_back(i);
			}
		}
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
