#include "Client.class.hpp"

void	Client::getRequest()
{
	_Request.getRequest(_pollFd.fd);
}

void	Client::getResponse()
{
	_Response.getResponse();
}

Client::Client(struct pollfd &pollFd, class ServerConfig& ServerConfig) :
	_pollFd(pollFd),
	_serverConfig(ServerConfig)
{
	static_cast<void>(_serverConfig);
}

Client::~Client(void)
{
}
