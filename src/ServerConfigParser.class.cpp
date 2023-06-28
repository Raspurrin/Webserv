#include "ServerConfigParser.hpp"
#include "colours.hpp"

ServerConfigParser::ServerConfigParser(std::string fileName)
{
	_fileToBeParsed.open(fileName);

	ServerConfig oneServerConfig = parsingOneServerConfig();
	addToVector(oneServerConfig);
}

void ServerConfigParser::removeCommentFrom(std::string &line)
{
	size_t  hashTagPosition = line.find('#');

	line = line.substr(0, hashTagPosition);
}

ServerConfig ServerConfigParser::parsingOneServerConfig()
{
	ServerConfig	oneServerConfig;
	std::string		line;

	while (getline(_fileToBeParsed, line))
	{
		removeCommentFrom(line);
		if (line[0] == '<')
			addRoute(_fileToBeParsed);
		else
			initializeConfiguration(oneServerConfig, line);
	}
	return (oneServerConfig);
}

ServerConfig::t_route ServerConfigParser::addRoute(std::ifstream &fileToBeParsed)
{
	(void)fileToBeParsed;
	ServerConfig::t_route newRoute;
	newRoute._methods = POST & DELETE;
	newRoute._directoryListing = true;
	newRoute._root = "bigman";
	newRoute._index = "somewhere";
	return (newRoute);
}

void	ServerConfigParser::initializeConfiguration(ServerConfig &oneServerConfig, std::string line)
{
	std::string		key; 
	std::string		value;
	std::string		firstWord;

	firstWord = trim(line);
	if (firstWord == "errorPage")
		addErrorPage(oneServerConfig, line);
	extractKeyValue(line, key, value);
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

	line = line.substr(9, line.length());
	extractKeyValue(line, key, value);
	oneServerConfig._errorPages[key] = value;
}

void    ServerConfigParser::addToVector(ServerConfig &oneServerConfig)
{
	_serverConfigs.push_back(oneServerConfig);
}

t_sockaddr_in   ServerConfigParser::setAddress(int port)
{
	t_sockaddr_in   address;

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
