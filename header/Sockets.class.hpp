#ifndef SOCKETS_CLASS_HPP
# define SOCKETS_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"

class Sockets
{
	private: 
		const int			port;
		struct sockaddr_in	address;
		std::vector<struct pollfd>	pollFds;
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
