#ifndef SERVERMANAGER_CLASS_HPP
# define SERVERMANAGER_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerConfig.class.hpp"

typedef struct pollfd					t_pollfd;
typedef struct route					t_route;
typedef struct server_config			t_server_config;

class ServerManager
{
	private:
		int								opt;
		std::vector<ServerConfig>		serverConfigs;
		std::vector<class Client>		clients;
		std::vector<t_pollfd>			sockets;
		std::vector<t_pollfd>			serverSockets;

	private:
		void	addServerSockets(void);
		void	addServerSocket(struct sockaddr_in serverAddress);
		void	configureSocket(int newSocket);
		void	addClientSocket(ServerConfig& serverConfig);
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
