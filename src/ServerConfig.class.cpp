#include "../header/ServerConfig.class.hpp"

// template<typename T>
// T	parsingKeyValue(std::stringstream Input)
// {

// }

bool	ServerConfig::checkRoutePath(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		return (true);
	return (false);
}

int		ServerConfig::getMethods(const std::string &path) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	return (routes.at(path).methods);
}

bool	ServerConfig::isMethodAllowed(const std::string &path, const int methodToCheck) const
{
	if (routes.find(path) != routes.end())
		error_handle("the route path does not exist");
	if (routes.at(path).methods & methodToCheck)
		return (true);
	return (false);
}

bool	ServerConfig::isDirListEnabled(const std::string &path) const
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

	// name(parsingKeyValue(orderedInput[0])),
	// clientBodySize(parsingKeyValue(orderedInput[1])),
	// errorPages(parsingKeyValue(orderdedInput[2])),
	// routes(parsingKeyValue(orderedInput[3]))
ServerConfig::ServerConfig(std::vector<std::stringstream> orderedInput) :
	name("tmp"),
	clientBodySize(300),
	errorPages({"404", "/errorpages"}),
	routes({"/CGI", {GET | POST | DELETE, true, "/newCGI"}}),
{
	// parsy parsy parsing
}

ServerConfig::ServerConfig(void)
{
}

//ServerConfig::ServerConfig(ServerConfig const &src);
//ServerConfig::ServerConfig&	operator=(ServerConfig const &assign);
ServerConfig::~ServerConfig(void)
{
	// Chaoss!
}
