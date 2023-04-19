#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"

class Server
{
	private:
		int									opt, serverFd, addressLen;
		std::map<std::string, std::string> configMap;
		std::vector<struct pollfd>			pollFds;
		std::vector<class Route>	routes;
		struct sockaddr_in			address;
		std::string					response;

	private:
		void	getRequest(int fd);
		void	postResponse(int fd);
		void	addNewConnection(int fd);

	public:
		void	checkConnections(void);

	public: 
		Server(void);
		//Server(Server const &src);
		//Server&	operator=(Server const &assign);
		~Server(void);
};

#endif
