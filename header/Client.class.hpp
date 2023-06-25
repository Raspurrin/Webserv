#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ServerConfig.class.hpp"
#include "webserv.hpp"
#include "Request.class.hpp"
#include "Response.class.hpp"

class Client
{
	private:
		struct pollfd			_pollFd;
		class ServerConfig&		_serverConfig;
		class Response			_Response;
		class Request			_Request;

	public:
		void					getRequest();
		void					getResponse();

	public:
		Client(struct pollfd& pollFd, class ServerConfig& ServerConfig);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
