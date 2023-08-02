#include "../header/ServerConfig.class.hpp"

ServerConfig::ServerConfig() :
	_port(0),
	_name(""),
	_clientBodySize(0)
{
	_errorPages.insert(std::make_pair("408", "default"));
    _errorPages.insert(std::make_pair("413", "default"));
    _errorPages.insert(std::make_pair("414", "default"));
    _errorPages.insert(std::make_pair("431", "default"));
}

int	ServerConfig::getPort() const
{
	return (_port);
}

std::string	ServerConfig::getName() const
{
	return (_name);
}

int		ServerConfig::getClientBodySize() const
{
	return (_clientBodySize);
}

std::string	ServerConfig::getErrorPage(std::string errorCode) const
{
	if (_errorPages.find(errorCode) != _errorPages.end())
		error_handle("the error code does not exist");
	if (_errorPages.at(errorCode) == "default")
		return ("default");
	return (_errorPages.at(errorCode));
}

void	ServerConfig::printErrorPages() const
{
	for (StringStringMap::const_iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
}

bool	ServerConfig::isRouteValid(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		return (false);
	return (true);
}

int		ServerConfig::getRouteMethods(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path)._methods);
}

bool	ServerConfig::isRouteMethodAllowed(const std::string &path, const int methodToCheck) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	if (_routes.at(path)._methods & methodToCheck)
		return (true);
	return (false);
}

bool	ServerConfig::isRouteDirListingEnabled(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path)._directoryListing);
}

const std::string	ServerConfig::getRouteRoot(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path)._root);
}

const std::string	ServerConfig::getRouteIndex(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path)._index);
}

t_sockaddr_in&		ServerConfig::getAddress()
{
	return (_address);
}

ServerConfig&	ServerConfig::operator=(ServerConfig const &assign)
{
	(void)assign;
	return (*this);
}

ServerConfig::~ServerConfig(void)
{
	// Chaoss!
}

void	ServerConfig::printServerConfig() const
{
	std::cout << "ServerConfig:" << std::endl;
	std::cout << "  port: " << _port << std::endl;
	std::cout << "  name: " << _name << std::endl;
	std::cout << "  clientBodySize: " << _clientBodySize << std::endl;
	std::cout << "  errorPages:" << std::endl;
	for (StringStringMap::const_iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
	std::cout << "  routes:" << std::endl;
	for (StringRouteMap::const_iterator it = _routes.begin(); it != _routes.end(); it++)
	{
		std::cout << "    " << it->first << ":" << std::endl;
		std::cout << "      methods: " << it->second._methods << std::endl;
		std::cout << "      directoryListing: " << it->second._directoryListing << std::endl;
		std::cout << "      HTTPRedirect: " << it->second._HTTPRedirect << std::endl;
		std::cout << "      root: " << it->second._root << std::endl;
		std::cout << "      index: " << it->second._index << std::endl;
	}
}

std::ostream&	operator<<(std::ostream &out, const ServerConfig &serverConfig)
{
	serverConfig.printServerConfig();
	return (out);
}