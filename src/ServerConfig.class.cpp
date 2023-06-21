#include "../header/ServerConfig.class.hpp"

// template<typename T>
// T	parsingKeyValue(std::stringstream Input)
// {

// }

int		ServerConfig::getServerSocketFd()
{
	return (_serverSocket.fd);
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
	return (_routes.at(path).dir_list);
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

struct sockaddr	ServerConfig::getServerAddress(void)
{
	return (_serverAddress);
}

ServerConfig::ServerConfig(const std::string& name, int clientBodySize, \
                			const std::map<std::string, std::string>& errorPages, \
                			const std::map<const std::string, const t_route>& routes, \
							struct sockaddr serverAddress) :
	_name(name),
	_clientBodySize(clientBodySize),
	_errorPages(errorPages),
	_routes(routes),
	_serverAddress(serverAddress)
{
	if (_clientBodySize)
		printf("hi\n");
	// parsy parsy parsing
}

ServerConfig::~ServerConfig(void)
{
	// Chaoss!
}
