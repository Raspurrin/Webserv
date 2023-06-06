#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ServerConfig.class.hpp"
#include "webserv.hpp"

class Client
{
	private:
		struct pollfd						pollFd;
		class ServerConfig&					serverConfig;
		class Response&						Response;
		class Request&						Request;
		// struct buffer					incomingBuffer;
		// struct buffer					outgoingBuffer;
		// std::map<std::string, std::string>	requestHeader;

	public:
		//std::vector<struct pollfd>			createPollFdVector(bool option, struct pollfd pollFd);
		int									getFd();
		int									getEvent();
		int									getRevent();
		class Server&						getServerRef();

	private:
		Client(void);

	public:
		Client(struct pollfd pollFd, class Server& Server);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
