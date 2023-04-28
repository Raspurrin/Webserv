#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ServerConfig.class.hpp"
#include "webserv.hpp"


class Client
{
	private: 
		struct buffer
		{
			std::string						buffer;
			bool							isDone;		
		};
		struct pollfd						pollFd;
		class ServerConfig&					serverConfig;
		struct buffer						incomingBuffer;
		struct buffer						outgoingBuffer;
		std::map<std::string, std::string>	requestHeader;

	public:
		std::vector<struct pollfd>			createPollFdVector(bool option, struct pollfd pollFd);
		int									getFd();
		int									getEvent();
		int									getRevent();
		class Server&						getServerRef();
		void								setRequestHeader(std::map<std::string, std::string>);

	private:
		Client(void);

	public:
		Client(struct pollfd pollFd, class Server& Server);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
