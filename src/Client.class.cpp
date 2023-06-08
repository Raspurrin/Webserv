#include "Client.class.hpp"

// struct pollfd				Client::getFd()
// {
// 	return(pollFd.fd);
// }

// int							Client::getEvent()
// {
// 	return(pollFd.event);
// }

// int							Client::getRevent()
// {
// 	return(pollFd.revent);
// }

// class Server&				Client::getServerRef()
// {
// 	return(Server);
// }

void	Client::getRequest()
{
	_Request.getRequest(_pollFd.fd);
}

void	Client::getReponse()
{
	_Response.getResponse(_pollFd.fd);
}

Client::Client(struct pollfd pollFd, class ServerConfig& ServerConfig, class Response& Response, class Request& Request) :
	_pollFd(pollFd),
	_serverConfig(ServerConfig),
	_Response(Response),
	_Request(Request)
{
}

// Client::Client(void)
// {
// }
//Client(Client const &src);
//Client&	operator=(Client const &assign);
Client::~Client(void)
{
}
