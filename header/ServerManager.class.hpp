#ifndef SERVERMANAGER_CLASS_HPP
# define SERVERMANAGER_CLASS_HPP

#include "webserv.hpp"
#include "colours.hpp"
#include "ServerConfig.class.hpp"
#include "ServerConfigParser.class.hpp"
#include "Client.class.hpp"

#define BACKLOG 0

typedef struct sockaddr_in			t_sockaddr_in;
typedef struct pollfd				t_pollfd;
typedef struct server_config		t_server_config;
typedef std::vector<t_pollfd>		PollFdVector;
typedef	std::map<int, class Client>	ClientMap;
typedef std::vector<ServerConfig>	ServerConfigVector;

class ServerManager
{
	private:
		int					_opt;
		int					_numServerSockets;
		ServerConfigVector	_serverConfigs;
		ClientMap		_clients;
		PollFdVector		_sockets;

	private:
		void	removeSockets(void);
		void	addServerSockets(void);
		void	addServerSocket(ServerConfig &serverConfig);
		void	configureSocket(int newSocket);
		void	addClientSocket(t_pollfd &serverSocket, ServerConfig &_serverConfigs);
		void	listenToServerSocket(int i);
		void	handleClientSocket(int i);
		void	sendResponse(Client &client);
		void	sendShutdownMessage(int clientSocket);

	public:
		void	eventLoop(void);

	public:
		ServerManager(void);
		ServerManager(const char*);
		//ServerManager(ServerManager const &src);
		//ServerManager&	operator=(ServerManager const &assign);
		~ServerManager(void);
};

#endif
