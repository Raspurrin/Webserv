	#include "../header/Server.class.hpp"

	std::string	validate_trim(std::string str, int (*check_func)(int))
	{
		if (std::all_of(str.begin(), str.end(), check_func) == false)
			error_handle("Configuration key or value should contain only characters");
		return (trim(str));
	}

	std::string	trim(std::string str)
	{
		int	first = str.find_first_not_of(" \t\n\r");
		int	last = str.find_last_not_of(" \t\n\r#");
		
		return (str.substr(first, last));
	}

	void	Server::parseConfFile(const char *confFileName)
	{
		int				equalSign;
		std::string		line, key, value;
		std::ifstream	confFile("confFileName");
		if (!confFile)
			error_handle("Wrong configuration file\n");
		do
		{
			line = trim(line);
			if (line.empty() || line[0] == '#')
				continue;
			if (line[0] == '[')
			{
				Route newRoute(line);
				Routes.push_back(newRoute);
				continue;
			}
			equalSign = line.find('=');
			key = line.substr(0, equalSign - 1);
			value = line.substr(equalSign + 1);
			key = validate_trim(key, isalpha);
			value = validate_trim(value, isalpha);
		} while (getline(confFile, line));
	}

	Server::Server(void) :
		port(8080),
		opt(1),
		addressLen(sizeof(address))
	{
		parseConfFile("webserver.conf");
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error_handle("Socket error");
		configureSocket(serverSocket);
		if (bind(serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
			error_handle("Binding error");
		if (listen(serverSocket, 3) < 0)
			error_handle("Listen error");
	}

	void	Server::configureSocket(int newSocket)
	{
		int		flags;

		if (setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
			error_handle("setsockopt");
		flags = fcntl(newSocket, F_GETFL, 0);
		fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
	}

	void	Server::addNewConnection(int newSocket)
	{
		pollfd	newPfd;

		configureSocket(newSocket);
		setsockopt(newSocket, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
		newPfd.fd = newSocket;
		newPfd.events = POLLIN | POLLOUT | POLLERR;
		pollFds.push_back(newPfd);
	}

	void	Server::getRequest(int new_socket)
	{
		char	buffer[30000];

		read(new_socket, buffer, 30000);
		printf("---------Buffer in Server is:\n %s\n--- BUFFER DONE\n", buffer);
		Request	request(buffer);
		request.printMap();
		response = request.getResponse();	
	}

	void	Server::postResponse(int socket, int indexToRemove)
	{
		send(socket, response.c_str(), response.length(), 0);
		printf("HELLO MESSAGE SENT FROM SERVER\n");
		pollFds.erase(pollFds.begin() + indexToRemove);
		close(socket);
	}

	void	Server::checkConnections(void)
	{
		int	newSocket;
		int	numEvent;

		while (69)
		{
			//printf("...WAITING FOR NEW CONNECTION...\n");
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

	// Server&	Server::operator=(Server const &assign)
	// {

	// }

	Server::~Server(void)
	{
		for (size_t i = 0; i < pollFds.size(); i++)
		{
			close(pollFds[i].fd);
		}
	}
