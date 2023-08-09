#include "../header/Client.class.hpp"

int		Client::getSocket()
{
	return (_pollFd.fd);
}

std::string Client::getResponse()
{
	return (_request.getResponse());
}

void	Client::getRequest(int& socketsIndex)
{
	_request.getRequest(_pollFd.fd, _serverConfig);
}

bool	Client::isRequestSent()
{
	return (_request.isFlagOn());
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
	this->_request = obj._request;
}

Client& Client::operator=(Client const& rhs)
{
	this->_pollFd = rhs._pollFd;
	this->_serverConfig = rhs._serverConfig;
	this->_request = rhs._request;
	return (*this);
}

StringStringMap Client::getHeaderFields() {
	return _request.getHeaderFields();
}

time_t Client::getLastActivity() {
	return _request.getLastActivity();
}

void Client::setRequestError(const ErrorResponse &error) {
	_request.setError(error);
}

bool Client::responseFinished() {
	return _request.responseFinished();
}
