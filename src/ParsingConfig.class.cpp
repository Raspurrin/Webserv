#include "ParsingConfig.hpp"
#include "colours.hpp"

 //hardcoded for testing xd

ParsingConfig::ParsingConfig(std::string fileName)
{
    std::ifstream _fileToBeParsed(fileName);
    std::string   buffer;

    while (getline(_fileToBeParsed, buffer, '#'))
    {
        ServerConfig oneServerConfig = parsingOneServerConfig();
        addToVector(oneServerConfig);
    }
}

ServerConfig ParsingConfig::parsingOneServerConfig()
{
    ServerConfig oneServerConfig;
    oneServerConfig._port = 8080;
    oneServerConfig._address = setAddress(oneServerConfig._port);
    oneServerConfig._name = "lol";
    oneServerConfig._clientBodySize = 69;
    oneServerConfig._errorPages["ha"] = "404";
    oneServerConfig._routes["newroute"] = addRoute();
    std::pair <std::string, ServerConfig::t_route> asdf = std::make_pair("newroute", addRoute());
    return (oneServerConfig);
    // oneServerConfig._routes.emplace("newroute", addRoute());
}

void    ParsingConfig::addToVector(ServerConfig &oneServerConfig)
{
    _serverConfigs.push_back(oneServerConfig);
}

t_sockaddr_in   ParsingConfig::setAddress(int port)
{
    t_sockaddr_in   address;

    address.sin_family = AF_INET;
   	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    return (address);
}

ServerConfig::t_route ParsingConfig::addRoute()
{
    ServerConfig::t_route newRoute;
    newRoute._methods = POST & DELETE;
    newRoute._directoryListing = true;
    newRoute._root = "bigman";
    newRoute._index = "somewhere";
    return (newRoute);
}

// =========================================================================================

std::string	ParsingConfig::trim(std::string str)
{
    if (str.empty())
        return ("");
    int	first = str.find_first_not_of(" \t\n\r\v\f");
    int	last = str.find_last_not_of(" \t\n\r\v\f");

    return (str.substr(first, last + 1));
}

std::string	ParsingConfig::validateTrim(std::string str, int (*checkFunc)(int))
{
    if (std::all_of(str.begin(), str.end(), checkFunc) == false)
        error_handle("Configuration key or value should contain only characters");
    return (trim(str));
}

void	ParsingConfig::extractKeyValue(std::string line, std::string &key, std::string &value)
{
    int	equalSign = line.find('=');

    key = line.substr(0, equalSign - 1);
    value = line.substr(equalSign + 1);
    key = trim(key);
    value = trim(value);
}

template <typename Key, typename Value>
void	ParsingConfig::printMap(const char *colourHeader, const char *colourBody, std::map<Key, Value> &map)
{
    std::cout << colourHeader << "\nPrinting map:\n" << DEF << std::endl;
    for (typename std::map<Key, Value>::iterator it = map.begin(); it != map.end(); ++it)
        std::cout << colourBody << it->first << " " << DEF << it->second << std::endl;
    return;
}

serverConfigVector    ParsingConfig::getServerConfigs()
{
    return (_serverConfigs);
}

ParsingConfig::~ParsingConfig(void)
{
}
