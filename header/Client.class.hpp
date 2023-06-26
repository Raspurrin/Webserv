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
		class Request			_Request;
		std::string				_responseMessage;

	public:
		int						getSocket();
		void					getRequest();
		std::string				getResponse();
		bool					isRequestSent();

	public:
		Client(struct pollfd& pollFd, class ServerConfig& ServerConfig);
		//Client(Client const &src);
		//Client&	operator=(Client const &assign);
		~Client(void);
};

#endif
