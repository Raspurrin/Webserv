#include "../header/ServerConfig.class.hpp"
#include "../header/colours.hpp"

ServerConfig::ServerConfig() :
	_host(0),
	_port(0),
	_name(""),
	_clientBodySize(0)
{
}

std::string ServerConfig::getHTTPRedirect(const std::string &path) const
{
	if (_routes.find(path) == _routes.end())
		return ("");
	return (_routes.at(path)._HTTPRedirect);
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
	if (_errorPages.find(errorCode) == _errorPages.end())
		return ("");
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
		return (true);
	return (false);
}

const StringRouteMap&	ServerConfig::getRoutesMap() const
{
	return (_routes);
}

int		ServerConfig::getRouteMethods(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path)._methods);
}

bool	ServerConfig::isRouteMethodAllowed(const std::string &path, const int methodToCheck) const
{
	if (_routes.find(path) == _routes.end())
		return (false);
	if (_routes.at(path)._methods & methodToCheck)
		return (true);
	return (false);
}

bool	ServerConfig::isRouteDirListingEnabled(const std::string &path) const
{
	if (_routes.find(path) == _routes.end())
		return (false);
	return (_routes.at(path)._directoryListing);
}

const std::string	ServerConfig::getRouteRoot(const std::string &path) const
{
	if (_routes.find(path) == _routes.end())
		return ("error");
	return (_routes.at(path)._root);
}

const std::string	ServerConfig::getRouteIndex(const std::string &path) const
{
	if (_routes.find(path) == _routes.end())
		return ("");
	return (_routes.at(path)._index);
}

std::set<std::string>	ServerConfig::getRouteCGI(const std::string &path) const
{
	if (_routes.find(path) == _routes.end())
		return (std::set<std::string>());
	return (_routes.at(path)._CGI);
}

t_sockaddr_in&		ServerConfig::getAddress()
{
	return (_address);
}

ServerConfig&	ServerConfig::operator=(ServerConfig const &assign)
{
	this->_host = assign._host;
	this->_name = assign._name;
	this->_port = assign._port;
	this->_clientBodySize = assign._clientBodySize;
	this->_errorPages = assign._errorPages;
	this->_routes = assign._routes;
	this->_address = assign._address;

	return (*this);
}

ServerConfig::~ServerConfig(void)
{
	// Chaoss!
}

void	ServerConfig::printServerConfig() const
{
	std::cout << PINK << "===============================" << std::endl;
	std::cout <<  "ServerConfig:" << RESET << std::endl;
	std::cout << CYAN << "  host: " << (_host >> 24) << "." << ((_host >> 16) & 255) << "." << ((_host >> 8) & 255) << "." << (_host & 255) << std::endl;
	std::cout << "  port: " << _port << std::endl;
	std::cout << "  name: " << _name << std::endl;
	std::cout << "  clientBodySize: " << _clientBodySize << std::endl;
	std::cout << GREEN << "  errorPages:" << CYAN << std::endl;
	for (StringStringMap::const_iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		std::cout << "    " << it->first << ": " << it->second << std::endl;
	std::cout << GREEN << "  routes:" << RESET << std::endl;
	for (StringRouteMap::const_iterator it = _routes.begin(); it != _routes.end(); it++)
	{
		std::cout << "    " << GREEN << it->first << ":" << CYAN << std::endl;
		std::cout << "      methods: " << it->second._methods << std::endl;
		std::cout << "      directoryListing: " << it->second._directoryListing << std::endl;
		std::cout << "      HTTPRedirect: " << it->second._HTTPRedirect << std::endl;
		std::cout << "      root: " << it->second._root << std::endl;
		std::cout << "      index: " << it->second._index << std::endl;
		for (std::set<std::string>::const_iterator setIt = it->second._CGI.begin(); setIt != it->second._CGI.end(); ++setIt)
        	std::cout << "	CGI: " << *setIt << std::endl;
	}
}

// std::ostream&	operator<<(std::ostream &out, const ServerConfig &serverConfig)
// {
// 	serverConfig.printServerConfig();
// 	return (out);
// }
