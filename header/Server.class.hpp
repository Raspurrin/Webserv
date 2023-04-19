#ifndef Server_CLASS_HPP
# define Server_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"

class Server
{
	private: 
		const int			port;
		struct sockaddr_in	address;
		std::vector<struct pollfd>	pollFds;
		int		opt, serverSocket, addressLen;
		std::string	response;

	private:
		void	getRequest(int fd);
		void	postResponse(int socket, int indexToRemove);
		void	configureSocket(int newSocket);
		void	addNewConnection(int newSocket);

	public:
		void	checkConnections(void);

	public:
		Server(void);
		//Socket(Socket const &src);
		//Server&	operator=(Server const &assign);
		~Server(void);
};

#endif
