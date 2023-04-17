#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "../header/Webserv.hpp"
#include "../header/colours.hpp"

class Sockets
{
	private: 
		const int			port;
		struct sockaddr_in	address;
		std::vector<struct pollfd>	lol;
		int		opt, serverFd, addressLen;

	private:
		void	readMessage(int fd);
		void	sendMessage(int fd);
		void	addNewConnection(int fd);

	public:
		void	checkConnections(void);

	public: 
		Sockets(void);
		//Socket(Socket const &src);
		//Sockets&	operator=(Sockets const &assign);
		~Sockets(void);
};

#endif
