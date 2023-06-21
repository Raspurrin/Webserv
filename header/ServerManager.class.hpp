#ifndef SERVERMANAGER_CLASS_HPP
# define SERVERMANAGER_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"
#include "../header/ServerConfig.class.hpp"

#define BACKLOG 3

typedef struct sockaddr_in				t_sockaddr_in;
typedef struct pollfd					t_pollfd;
typedef struct route					t_route;
typedef struct server_config			t_server_config;

class ServerManager
{
	private:
		typedef struct serverSocket
		{
			t_pollfd		poll;
			t_sockaddr_in	address;
		}	t_serverSocket;
		int								opt;
		std::vector<ServerConfig>		serverConfigs;
		std::vector<class Client>		clients;
		std::vector<t_pollfd>			clientSockets;
		std::vector<t_serverSocket>		serverSockets;

	private:
		void	addServerSockets(void);
		void	addServerSocket(ServerConfig &serverConfig);
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
