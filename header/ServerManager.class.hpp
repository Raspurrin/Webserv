#ifndef SERVERMANAGER_CLASS_HPP
# define SERVERMANAGER_CLASS_HPP

#include "webserv.hpp"
#include "colours.hpp"
#include "ServerConfig.class.hpp"
#include "ParsingConfig.hpp"
#include "Client.class.hpp"

#define BACKLOG 3

typedef struct sockaddr_in			t_sockaddr_in;
typedef struct pollfd				t_pollfd;
typedef struct route				t_route;
typedef struct server_config		t_server_config;
typedef std::vector<t_pollfd>		PollFdVector;
typedef	std::vector<class Client>	ClientVector;
typedef std::vector<ServerConfig>	ServerConfigVector;

class ServerManager
{
	private:
		int					opt;
		ServerConfigVector	serverConfigs;
		ClientVector		clients;
		PollFdVector		clientSockets;
		PollFdVector		serverSockets;

	private:
		void	addServerSockets(void);
		void	addServerSocket(ServerConfig &serverConfig);
		void	configureSocket(int newSocket);
		void	addClientSocket(t_pollfd &serverSocket, ServerConfig &serverConfigs);
		void	listenToServerSockets();
		void	handleClientSockets();

	public:
		void	eventLoop(void);

	public:
		ServerManager(void);
		//ServerManager(ServerManager const &src);
		//ServerManager&	operator=(ServerManager const &assign);
		~ServerManager(void);
};

#endif
