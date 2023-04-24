#include "../header/ServerConfig.class.hpp"

bool		ServerConfig::checkRoutePath(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		return (true);
	return (false);
}

int			ServerConfig::getMethods(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	return (routes.at(path).methods);
}

bool		ServerConfig::isMethodAllowed(const std::string &path, const int methodToCheck) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	if (routes.at(path).methods & methodToCheck)
		return (true);
	return (false);
}

bool		ServerConfig::isDirListEnabled(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	return (routes.at(path).dir_list);
}

const std::string	ServerConfig::getRouteRoot(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	return (routes.at(path).root);
}

const std::string	ServerConfig::getRouteIndex(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	return (routes.at(path).index);
}

ServerConfig::ServerConfig(void) :
	name(),
	clientBodySize(),
	errorPages(),
	routes()
{
	// parsy parsy paring
}

//ServerConfig::ServerConfig(ServerConfig const &src);
//ServerConfig::ServerConfig&	operator=(ServerConfig const &assign);
ServerConfig::~ServerConfig(void)
{
	// Chaoss!
}
