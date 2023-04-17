#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../header/Webserv.hpp"
#include "../header/colours.hpp"

typedef struct pollfd	yo;

class Sockets
{
	private: 
		const int			port;
		struct sockaddr_in	address;
		std::vector<pollfd>	lol;
		int					opt, serverFd, addressLen, readed;

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
