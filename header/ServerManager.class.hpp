#ifndef SERVERMANAGER_CLASS_HPP
# define SERVERMANAGER_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"

typedef struct pollfd					t_pollfd;
typedef struct route					t_route;
typedef struct server_config			t_server_config;

class ServerManager
{
	private:
		int								opt;
		std::vector<t_server_config>	serverConfigs;
		std::list<class Client>			clients;
		std::vector<t_pollfd>			sockets;
		std::vector<t_pollfd>			serverSockets;
		struct sockaddr_in				address;
		int								addressLen;
		Response&						response;
		Request&						request;

	public:
		void	eventLoop(void);

	private:
		void	parseConfigFile(void);
		void	addServerSocket(void);
		void	configureSocket(int newSocket);
		void	addClientSocket(int newSocket);
		void	getRequest(int fd);
		void	postResponse(int socket, int indexToRemove);

	public:
		ServerManager(void);
		//ServerManager(ServerManager const &src);
		//ServerManager&	operator=(ServerManager const &assign);
		~ServerManager(void);
};

#endif
