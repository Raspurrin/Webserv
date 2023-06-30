#include "../header/ServerConfigParser.class.hpp"
#include "colours.hpp"

ServerConfigParser::ServerConfigParser(std::string fileName)
{
	_fileToBeParsed.open(fileName);

	if (!_fileToBeParsed.is_open())
		throw std::invalid_argument("File not found");
	while (!_fileToBeParsed.eof())
	{
		if (!checkForServerDeclaration())
			break;
		ServerConfig oneServerConfig = parsingOneServerConfig();
		addToVector(oneServerConfig);
	}
}

bool	ServerConfigParser::checkForServerDeclaration()
{
	std::string		line;
	bool			serverCheck;

	std::cout << "Checking for server declaration" << std::endl;
	while (getline(_fileToBeParsed, line))
	{
		removeCommentFrom(line);
		if (trim(line) == "server")
			serverCheck = true;
		if (trim(line) == "{" && serverCheck == true)
			return (true);
	}
	return (false);
}

void ServerConfigParser::removeCommentFrom(std::string &line)
{
	std::cout << "Removing comments from line: " << line << std::endl;
	size_t  hashTagPosition = line.find('#');

	line = line.substr(0, hashTagPosition);
}

ServerConfig ServerConfigParser::parsingOneServerConfig()
{
	ServerConfig	oneServerConfig;
	std::string		line;

	std::cout << CYAN << "Parsing one server config" << DEF << std::endl;
	while (getline(_fileToBeParsed, line) && trim(line) != "}")
	{
		removeCommentFrom(line);
		if (line.empty())
			continue ;
		if (trim(line)[0] == '<')
			addRoute(_fileToBeParsed, oneServerConfig);
		else
			initializeConfiguration(oneServerConfig, line);
	}
	return (oneServerConfig);
}

std::string 	ServerConfigParser::getRouteName(std::ifstream &fileToBeParsed)
{
	std::string		line;
	std::string		routeName;

	std::cout << "Getting route name" << std::endl;
	getline(fileToBeParsed, line);
	routeName = line.substr(1, line.length() - 1);
	return (routeName);
}

ServerConfig::route ServerConfigParser::addRoute(std::ifstream &fileToBeParsed, ServerConfig &oneServerConfig)
{
	std::string				line;
	ServerConfig::route		newRoute;
	std::string				routeName;
	(void)fileToBeParsed;

	std::cout << "Adding route" << std::endl;
	routeName = getRouteName(fileToBeParsed);
	while (getline(fileToBeParsed, line) && trim(line).substr(0, 2) != "</")
		initializeRoute(line, newRoute);
	
	oneServerConfig._routes[routeName] = newRoute;
	return (newRoute);
}

void	ServerConfigParser::initializeRoute(std::string line, ServerConfig::route newRoute)
{
	std::string		key; 
	std::string		value;

	std::cout << CYAN << "Initializing route: " << line << DEF << std::endl;
	extractKeyValue(line, key, value);
	if (key == "allowedMethod")
		addAllowedMethod(value, newRoute._methods);
	else if (key == "directoryListing")
		newRoute._directoryListing = checkBooleanString(value);
	else if (key == "root")
		newRoute._root = value;
	else if (key == "index")
		newRoute._index = value;
}

bool	ServerConfigParser::checkBooleanString(std::string boolString)
{
	std::cout << "Checking boolean string" << std::endl;
	if (boolString == "on")
		return (true);
	else if (boolString == "off")
		return (false);
	else
		throw std::invalid_argument("Invalid argument for boolean value");
}

void	ServerConfigParser::addAllowedMethod(std::string method, int &allowedMethod)
{
	std::cout << "Adding allowed method" << std::endl;
	if (method == "GET")
		allowedMethod |= GET;
	else if (method == "POST")
		allowedMethod |= POST;
	else if (method == "DELETE")
		allowedMethod |= DELETE;
}

void	ServerConfigParser::initializeConfiguration(ServerConfig &oneServerConfig, std::string line)
{
	std::string		key; 
	std::string		value;
	std::string		firstWord;

	std::cout << YELLOW << "Initializing configuration - ";
	firstWord = trim(line);
	if (firstWord == "errorPage")
		addErrorPage(oneServerConfig, line);
	extractKeyValue(line, key, value);
	std::cout << "key: " << key << " value: " << value << DEF << std::endl;
	if (key == "port")
		oneServerConfig._port = std::stoi(value);
	else if (key == "serverName")
		oneServerConfig._name = value;
	else if (key == "clientBodySize")
		oneServerConfig._clientBodySize = std::stoi(value);
}

void	ServerConfigParser::addErrorPage(ServerConfig &oneServerConfig, std::string line)
{
	std::string		key; 
	std::string		value;

	std::cout << "Adding error page" << std::endl;
	line = line.substr(9, line.length());
	extractKeyValue(line, key, value);
	oneServerConfig._errorPages[key] = value;
}

void    ServerConfigParser::addToVector(ServerConfig &oneServerConfig)
{
	std::cout << "Adding to vector" << std::endl;
	_serverConfigs.push_back(oneServerConfig);
}

t_sockaddr_in   ServerConfigParser::setAddress(int port)
{
	t_sockaddr_in   address;

	std::cout << "Setting address" << std::endl;
	address.sin_family = AF_INET;
   	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	return (address);
}

// =========================================================================================

std::string	ServerConfigParser::trim(std::string str)
{
	if (str.empty())
		return ("");
	int	first = str.find_first_not_of(" \t\n\r\v\f");
	int	last = str.find_last_not_of(" \t\n\r\v\f");

	return (str.substr(first, last + 1));
}

std::string	ServerConfigParser::validateTrim(std::string str, int (*checkFunc)(int))
{
	if (std::all_of(str.begin(), str.end(), checkFunc) == false)
		error_handle("Configuration key or value should contain only characters");
	return (trim(str));
}

void	ServerConfigParser::extractKeyValue(std::string line, std::string &key, std::string &value)
{
	int	equalSign = line.find('=');

	if (equalSign == -1)
		error_handle("Configuration key and value should be separated by an equal sign");
	else if (equalSign + 1 >= (int)line.length())
		error_handle("Configuration value should not be empty");
	else if (equalSign == 0)
		error_handle("Configuration key should not be empty");
	key = line.substr(0, equalSign - 1);
	value = line.substr(equalSign + 1);
	key = trim(key);
	value = trim(value);
}

template <typename Key, typename Value>
void	ServerConfigParser::printMap(const char *colourHeader, const char *colourBody, std::map<Key, Value> &map)
{
	std::cout << colourHeader << "\nPrinting map:\n" << DEF << std::endl;
	for (typename std::map<Key, Value>::iterator it = map.begin(); it != map.end(); ++it)
		std::cout << colourBody << it->first << " " << DEF << it->second << std::endl;
	return;
}

serverConfigVector    ServerConfigParser::getServerConfigs()
{
	return (_serverConfigs);
}

ServerConfigParser::~ServerConfigParser(void)
{
}