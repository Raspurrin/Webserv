#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ServerConfig.class.hpp"
#include "webserv.hpp"


class Client
{

	private: 
		struct pollfd	pollFd;
		class Server&	Server;

	public:
		std::vector<struct pollfd>	createPollFdVector(bool option, struct pollfd pollFd);
		int							getFd();
		int							getEvent();
		int							getRevent();
		class Server&				getServerRef();

	private:
		Client(void);

	public:
		Client(struct pollfd pollFd, class Server& Server);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
