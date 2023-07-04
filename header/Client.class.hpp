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
		class Request			_request;
		std::string				_responseMessage;

	public:
		int						getSocket();
		void					getRequest();
		std::string				getResponse();
		bool					isRequestSent();
		StringStringMap			getHeaderFields();

	public:
		Client(struct pollfd& pollFd, class ServerConfig& ServerConfig);
		Client(Client const &obj);
		Client&	operator=(Client const &rhs);
		~Client(void);
};

#endif
