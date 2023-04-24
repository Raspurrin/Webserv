	#include "../header/ServerManager.class.hpp"
	
	ServerManager::ServerManager(void) :
		opt(1)
	{
		int	addressLen;
	
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(8080);
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket);
		if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
			error_handle("Binding error");
		if (listen(serverSocket, 3) < 0)
			error_handle("Listen error");
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

	void	

	void	ServerManager::addClientSocket(int newSocket)
	{
		pollfd	newPfd;

		configureSocket(newSocket);
		setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.fd = newSocket;
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		pollFds.push_back(newPfd);
	}

	void	ServerManager::getRequest(int new_socket)
	{
		char	buffer[30000];

		read(new_socket, buffer, 30000);
		printf("---------Buffer in ServerManager is:\n %s\n--- BUFFER DONE\n", buffer);
		Request	request(buffer);
		request.printMap();
		response = request.getResponse();	
	}

	void	ServerManager::postResponse(int socket, int indexToRemove)
	{
		send(socket, response.c_str(), response.length(), 0);
		printf("HELLO MESSAGE SENT FROM ServerManager\n");
		pollFds.erase(pollFds.begin() + indexToRemove);
		close(socket);
	}

	void	ServerManager::eventLoop(void)
	{
		int	newSocket;
		int	numEvent;

		while (69)
		{
			printf("...WAITING FOR NEW CONNECTION...\n");
			newSocket = accept(serverSocket, (struct sockaddr *) &address, (socklen_t *) &addressLen);
			if (newSocket > 0)
				addNewConnection(newSocket);
			numEvent = poll(pollFds.data(), pollFds.size(), 300);
			if (numEvent > 0)
			{
				for (size_t i = 0; i < pollFds.size(); i++)
				{
					if (pollFds[i].revents & POLLIN)
						getRequest(pollFds[i].fd);
					else if (pollFds[i].revents & POLLOUT)
						postResponse(pollFds[i].fd, i);
					else if (pollFds[i].revents & POLLERR)
						error_handle("Error occured with a connection");
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
		for (size_t i = 0; i < pollFds.size(); i++)
		{
			close(pollFds[i].fd);
		}
	}
