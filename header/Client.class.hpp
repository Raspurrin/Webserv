#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ServerConfig.class.hpp"
#include "webserv.hpp"

class Client
{
	private:
		struct pollfd						_pollFd;
		class ServerConfig&					_serverConfig;
		class Response						_Response;
		class Request						_Request;
		// struct buffer					incomingBuffer;
		// struct buffer					outgoingBuffer;
		// std::map<std::string, std::string>	requestHeader;

	public:
		//std::vector<struct pollfd>		createPollFdVector(bool option, struct pollfd pollFd);
		// int								getFd();
		// int								getEvent();
		// int								getRevent();
		// class Server&					getServerRef();
		void								getRequest();
		void								getResponse();

	// private:
	// 	Client(void);

	public:
		Client(struct pollfd pollFd, class ServerConfig& ServerConfig, class Response& Response, class Request& Request);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
