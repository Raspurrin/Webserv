#include "../header/ServerManager.class.hpp"
#include "../header/ServerConfigParser.class.hpp"
#include "../header/Cgi.class.hpp"
#include <sys/poll.h>
#include <unistd.h>
#include <algorithm>

IntVector	_fdsToRemove;

ServerManager::ServerManager(const char* configuration_file) :
	_opt(1), _numServerSockets(0)
{
	ServerConfigParser	parsingConfig(configuration_file);

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
		if (g_shutdown_flag)
		{
			for (size_t i = 0; i < _sockets.size(); ++i)
			{
				close(_sockets[i].fd);
				_fdsToRemove.push_back(_sockets[i].fd);
			}
			removeSockets();
			break ;
		}
		numEvent = poll(_sockets.data(), _sockets.size(), -1);
		if (numEvent <= 0)
			continue ;
		for (size_t i = 0; i < _sockets.size(); ++i)
		{
			if (i < (size_t)_numServerSockets)
				listenToServerSocket(i);
			else
				handleClientSocket(i);
		}
		removeSockets();
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
		newPfd.events = POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL;
		newPfd.revents = 0;
		_sockets.push_back(newPfd);
		Client	newClient(newPfd, serverConfig);
		_clients[newPfd.fd] = newClient;
	}
}

void	ServerManager::sendResponse(Client &client)
{
	std::string	_response = client.getResponse();
	static int i = 0;

	if (DEBUG)
		std::cout << YELLOW << "========= " << DEF << "Sending response: " << i << YELLOW << " =========\n\n" << DEF;

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
	t_pollfd&		socketPoll = _sockets[i];
	class Client&	clientPoll = _clients[socketPoll.fd];

	if (socketPoll.revents & POLLERR || socketPoll.revents & POLLHUP || socketPoll.revents & POLLNVAL) {
		close(socketPoll.fd);
		_fdsToRemove.push_back(socketPoll.fd);
		return ;
	}
	if (!clientPoll.isRequestSent() && time(NULL) - clientPoll.getLastActivity() > REQUEST_TIMEOUT) {
		clientPoll.setRequestError(ErrorResponse(408, "from ServerManager"));
	}
	if (socketPoll.revents & POLLIN && !clientPoll.isRequestSent())
	{
		if (DEBUG)
			std::cout << "- POLLIN with index " << i << " fd is " << _sockets[i].fd << std::endl;

		clientPoll.getRequest();
	}
	else if (clientPoll.isRequestSent() && socketPoll.revents & POLLOUT) {
		sendResponse(clientPoll);
		if (clientPoll.responseFinished()) {
			_fdsToRemove.push_back(socketPoll.fd);
		}
	}
}

void	ServerManager::removeSockets()
{
	PollFdVector::iterator it = _sockets.begin();
	while (it != _sockets.end()) {
		if (std::find(_fdsToRemove.begin(), _fdsToRemove.end(), it->fd) != _fdsToRemove.end()) {
			_clients.erase(it->fd);
			it = _sockets.erase(it);
		} else {
			it++;
		}
	}

	_fdsToRemove.clear();
}

ServerManager::~ServerManager(void)
{
}
