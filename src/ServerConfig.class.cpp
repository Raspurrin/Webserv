#include "../header/ServerConfig.class.hpp"

// template<typename T>
// T	parsingKeyValue(std::stringstream Input)
// {

// }


int		ServerConfig::getPort() const
{
	return (_port);
}

bool	ServerConfig::checkRoutePath(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		return (true);
	return (false);
}

int		ServerConfig::getMethods(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path).methods);
}

bool	ServerConfig::isMethodAllowed(const std::string &path, const int methodToCheck) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	if (_routes.at(path).methods & methodToCheck)
		return (true);
	return (false);
}

bool	ServerConfig::isDirListEnabled(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path).directoryListing);
}

const std::string	ServerConfig::getRouteRoot(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path).root);
}

const std::string	ServerConfig::getRouteIndex(const std::string &path) const
{
	if (_routes.find(path) != _routes.end())
		error_handle("the route path does not exist");
	return (_routes.at(path).index);
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
