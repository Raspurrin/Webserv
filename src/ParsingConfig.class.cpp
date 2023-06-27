 #include "../header/ParsingConfig.hpp"

 //hardcoded for testing xd

t_sockaddr_in   ParsingConfig::setAddress(int port)
{
    t_sockaddr_in   address;

    address.sin_family = AF_INET;
   	address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    return (address);
}

serverConfigVector    ParsingConfig::parsing(std::string fileName)
{
    fileToBeParsed.open(fileName.c_str());
    ServerConfig oneServerConfig = parsingOneServerConfig();
    addToVector(oneServerConfig);
    return (serverConfigs);
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

ServerConfig::t_route ParsingConfig::addRoute()
{
    ServerConfig::t_route newRoute;
    newRoute.methods = POST & DELETE;
    newRoute.directoryListing = true;
    newRoute.root = "bigman";
    newRoute.index = "somewhere";
    return (newRoute);
}

void    ParsingConfig::addToVector(ServerConfig &oneServerConfig)
{
    serverConfigs.push_back(oneServerConfig);
}

ParsingConfig::ParsingConfig(void)
{
}

ParsingConfig::~ParsingConfig(void)
{
}
