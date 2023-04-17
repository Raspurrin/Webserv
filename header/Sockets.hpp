#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/socket.h>
#include "../header/Webserv.hpp"
#include "../header/colours.hpp"

class Sockets
{
	private: 
		const int			port;
		struct sockaddr_in	address;
		int					opt, server_fd, address_len, readed;
		char				buffer[30000];

	public:
		void	checkConnections(void);

	public: 
		Sockets(void);
		//Socket(Socket const &src);
		//Sockets&	operator=(Sockets const &assign);
		~Sockets(void);
};

#endif
