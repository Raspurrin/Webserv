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
	return (_Request.isFlagOn());
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

Client::Client(Client const& obj) : _serverConfig(obj._serverConfig)
{
	this->_pollFd = obj._pollFd;
	this->_Request = obj._Request;
	this->_responseMessage = obj._responseMessage;
}

Client& Client::operator=(Client const& rhs)
{
	this->_pollFd = rhs._pollFd;
	this->_serverConfig = rhs._serverConfig;
	this->_Request = rhs._Request;
	this->_responseMessage = rhs._responseMessage;
	return (*this);
}
