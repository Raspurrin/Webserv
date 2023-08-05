#include "../header/ServerConfigParser.class.hpp"
#include "colours.hpp"

ServerConfigParser::ServerConfigParser(const char *fileName)
{
	_fileToBeParsed.open(fileName);

	try
	{
		if (!_fileToBeParsed.is_open())
			throw std::invalid_argument("File not found");
		while (!_fileToBeParsed.eof())
		{
			if (!checkForServerDeclaration())
				break;
			std::cout << RED << "\nServer declaration found" << DEF << std::endl;
			ServerConfig oneServerConfig = parsingOneServerConfig();
			checkMinimumConfiguration(oneServerConfig);
			setAddress(oneServerConfig);
			std::cout << RED << "\nInside Parser:" << DEF << std::endl;
			oneServerConfig.printServerConfig();
			addToVector(oneServerConfig);
		}
		if (_serverConfigs.empty())
			throw std::invalid_argument("No server declaration found");
	}
	catch (std::exception &e)
	{
		std::cerr << "configuration failed: " << e.what() << std::endl;
		exit(0);
	}
}

void	ServerConfigParser::checkMinimumConfiguration(ServerConfig &oneServerConfig)
{
	std::cout << "Checking minimum configuration" << std::endl;
	if (oneServerConfig._port == 0)
		throw std::invalid_argument("Port not set");
	if (oneServerConfig._name.empty())
		throw std::invalid_argument("Server name not set");
	if (oneServerConfig._clientBodySize == 0)
		throw std::invalid_argument("Client body size not set");
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
		else if (trim(line) == "{" && serverCheck == true)
			return (true);
		else if (trim(line) != "")
			throw std::invalid_argument("garbage detected in between server declarations");
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
		std::cout << "before removeCommentFrom: " << line << std::endl;
		removeCommentFrom(line);
		std::cout << "in parsingOneServerConfig: " << line << std::endl;
		if (trim(line) == "")
			continue ;
		if (trim(line)[0] == '<')
			addRoute(line, oneServerConfig);
		else
		{
			std::cout << "about to initialize configuration" << std::endl;
			initializeConfiguration(oneServerConfig, line);
		}
	}

	return (oneServerConfig);
}

std::string 	ServerConfigParser::getRouteName(std::string &firstLine)
{
	std::string		routeName;

	routeName = firstLine.substr(2, firstLine.length() - 3);
	std::cout << "Getting route name: " << routeName << std::endl;
	return (routeName);
}

ServerConfig::route ServerConfigParser::addRoute(std::string firstLine, ServerConfig &oneServerConfig)
{
	std::string				line;
	ServerConfig::route		newRoute;
	std::string				routeName;

	std::cout << "Adding route" << std::endl;
	routeName = getRouteName(firstLine);
	while (getline(_fileToBeParsed, line) && trim(line).substr(0, 2) != "</")
		initializeRoute(line, newRoute);
	
	oneServerConfig._routes[routeName] = newRoute;
	std::cout << "newRoute method: " << newRoute._methods << std::endl;
	std::cout << RED << "in addRoute: " << std::endl;
	return (newRoute);
}

void	ServerConfigParser::initializeRoute(std::string line, ServerConfig::route &newRoute)
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
		setRouteRoot(value, newRoute);
	else if (key == "index")
		setRouteIndex(value, newRoute);
	else if (key == "HTTPRedirect")
		setRouteHTTPRedirect(value, newRoute);
	else 
		throw std::invalid_argument("Invalid configuration key");
}

void	ServerConfigParser::setRouteHTTPRedirect(std::string &value, ServerConfig::route &newRoute)
{
	std::cout << "Setting route HTTP redirect" << std::endl;
	if (value.empty())
		throw std::invalid_argument("HTTP redirect cannot be empty");
	if (!newRoute._HTTPRedirect.empty())
		throw std::invalid_argument("HTTP redirect already set");
	newRoute._HTTPRedirect = value;
}

void	ServerConfigParser::setRouteRoot(std::string &root, ServerConfig::route &newRoute)
{
	if (root.empty())
		throw std::invalid_argument("Root cannot be empty");
	if (!newRoute._root.empty())
		throw std::invalid_argument("Root already set");
	newRoute._root = root;
}

void	ServerConfigParser::setRouteIndex(std::string &index, ServerConfig::route &newRoute)
{
	if (index.empty())
		throw std::invalid_argument("Index cannot be empty");
	if (!newRoute._index.empty())
		throw std::invalid_argument("Index already set");
	newRoute._index = index;
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
	else
		throw std::invalid_argument("Invalid argument for allowed method");
	std::cout << "Allowed method: " << allowedMethod << std::endl;
}

void	ServerConfigParser::initializeConfiguration(ServerConfig &oneServerConfig, std::string line)
{
	std::string		key; 
	std::string		value;
	std::string		firstWord;

	std::cout << YELLOW << "Initializing configuration - ";
	firstWord = findFirstWord(line);
	std::cout << YELLOW << "firstWord: " << "!" << firstWord << "!" DEF << std::endl;
	extractKeyValue(line, key, value);
	std::cout << YELLOW << "key: " << key << " value: " << value << DEF << std::endl;
	if (firstWord == "errorPage")
	{
		std::cout << "errorPage found" << std::endl;
		addErrorPage(oneServerConfig, line);
	}
	else if (key == "host")
		setHost(value, oneServerConfig);
	else if (key == "port")
		setPort(value, oneServerConfig);
	else if (key == "serverName")
		setServerName(value, oneServerConfig);
	else if (key == "clientBodySize")
		setClientBodySize(value, oneServerConfig);
	else
		std::cout << "Invalid configuration key" << std::endl;
}

void 	ServerConfigParser::setHost(std::string &value, ServerConfig &oneServerConfig)
{
	std::stringstream ss(value);
	std::string nbr;
	size_t iteration;
	if (oneServerConfig._host != 0)
		throw std::invalid_argument("Host already set");
    for (iteration =  0; getline (ss, nbr, '.'); iteration++)
	{
		if (!validate(nbr, isdigit))
			throw std::invalid_argument("Host should be a number");
		int nbrInt = std::atoi(nbr.c_str());
		if (nbrInt < 0 || nbrInt > 255)
			throw std::invalid_argument("Invalid host number");
		oneServerConfig._host = (oneServerConfig._host << 8) | nbrInt;
	}
	if (iteration != 4)
		throw std::invalid_argument("Invalid host");

	std::cout << BLACK << "Setting host: " << oneServerConfig._host << DEF << std::endl;
}

void	ServerConfigParser::setPort(std::string &value, ServerConfig &oneServerConfig)
{
	if (!validate(value, isdigit))
		throw std::invalid_argument("Port should be a number");
	int port = std::atoi(value.c_str());
	std::cout << BLACK << "Setting port: " << port << DEF << std::endl;
	if (oneServerConfig._port != 0)
		throw std::invalid_argument("Port already set");
	if (port < 0 || port > 65535)
		throw std::invalid_argument("Invalid port number");
	oneServerConfig._port = port;
}

void	ServerConfigParser::setServerName(std::string &serverName, ServerConfig &oneServerConfig)
{
	if (!validate(serverName, isalnum))
		throw std::invalid_argument("Server name should be alphanumeric");
	std::cout << BLACK << "Setting server name: " << serverName << DEF << std::endl;
	if (serverName.empty())
		throw std::invalid_argument("Server name cannot be empty");
	if (!oneServerConfig._name.empty())
		throw std::invalid_argument("Server name already set");
	checkForServerNameDuplicate(serverName);
	oneServerConfig._name = serverName;
}

void	ServerConfigParser::checkForServerNameDuplicate(const std::string &serverName) const
{
	std::cout << "Checking for server name duplicate" << std::endl;
	for (serverConfigVector::const_iterator it = _serverConfigs.begin(); it != _serverConfigs.end(); ++it)
		if (it->_name == serverName)
			throw std::invalid_argument("Server name already exists");
}

void 	ServerConfigParser::setClientBodySize(std::string &value, ServerConfig &oneServerConfig)
{
	if (!validate(value, isdigit))
		throw std::invalid_argument("Client body size should be a number");
	int clientBodySize = std::atoi(value.c_str());
	std::cout << BLACK << "Setting client body size: " << clientBodySize << DEF << std::endl;
	if (clientBodySize < 0)
		throw std::invalid_argument("Client body size cannot be negative");
	if (oneServerConfig._clientBodySize != 0)
		throw std::invalid_argument("Client body size already set");
	else
		oneServerConfig._clientBodySize = clientBodySize;
}

void	ServerConfigParser::addErrorPage(ServerConfig &oneServerConfig, std::string line)
{
	std::string		key;
	int				keyInt;
	std::string		value;

	std::cout << "Adding error page" << std::endl;
	line = line.substr(10, line.length());
	extractKeyValue(line, key, value);
	
	keyInt = std::atoi(key.c_str());
	std::cout << "key: " << key << " value: " << value << std::endl;
	if (keyInt < 100 || keyInt > 599)
		throw std::invalid_argument("Invalid error code for error page");
	if (oneServerConfig._errorPages.find(key) != oneServerConfig._errorPages.end() && oneServerConfig._errorPages[key] != "default")
		throw std::invalid_argument("error page already set");
	if (value == "default")
		throw std::invalid_argument("cannot set error page to default");
	oneServerConfig._errorPages[key] = value;
}

void    ServerConfigParser::addToVector(ServerConfig &oneServerConfig)
{
	std::cout << "Adding to vector" << std::endl;
	_serverConfigs.push_back(oneServerConfig);
}

void   ServerConfigParser::setAddress(ServerConfig &oneServerConfig)
{
	std::cout << "Setting address" << std::endl;
	oneServerConfig._address.sin_family = AF_INET;
   	oneServerConfig._address.sin_addr.s_addr = oneServerConfig._host;
	oneServerConfig._address.sin_port = htons(oneServerConfig._port);
}

// =========================================================================================

std::string	ServerConfigParser::findFirstWord(std::string line)
{
	int	first = line.find_first_not_of(" \t\n\r\v\f");
	int	last = line.find_first_of(" \t\n\r\v\f", first + 1);
	if (first == -1 || last == -1)
		return ("");
	return (line.substr(first, last - 1));
}

std::string	ServerConfigParser::trim(std::string str)
{
	int	first = str.find_first_not_of(" \t\n\r\v\f");
	int	last = str.find_last_not_of(" \t\n\r\v\f");
	if (first == -1 || last == -1)
		return ("");
	return (str.substr(first, last + 1));
}

bool		ServerConfigParser::validate(std::string str, int (*checkFunc)(int))
{
	for (int i = 0; i < (int)str.length(); i++)
		if (!checkFunc(str[i]))
			return (false);
	return (true);
}

std::string	ServerConfigParser::validateTrim(std::string str, int (*checkFunc)(int))
{
	if (!validate(str, checkFunc))
		throw std::invalid_argument("Invalid argument");
	return (trim(str));
}

void	ServerConfigParser::extractKeyValue(std::string line, std::string &key, std::string &value)
{
	int	equalSign = line.find('=');

	if (equalSign == -1)
		throw std::invalid_argument("Configuration key and value should be separated by an equal sign");
	else if (equalSign + 1 >= (int)line.length())
		throw std::invalid_argument("Configuration value should not be empty");
	else if (equalSign == 0)
		throw std::invalid_argument("Configuration key should not be empty");
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
