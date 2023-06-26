#include "Client.class.hpp"

int		Client::getSocket()
{
	return (_pollFd.fd);
}

std::string Client::getResponse()
{
	return (_responseMessage);
}

void	Client::getRequest()
{
	_responseMessage = _Request.getRequest(_pollFd.fd);
	std::cout << "responseMessaage: " << _responseMessage << std::endl;
}

bool	Client::isRequestSent()
{
	return (_Request.isFlagOn(REACHED_BODY_END));
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
