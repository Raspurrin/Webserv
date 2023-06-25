 #include "ParsingConfig.hpp"

 //hardcoded for testing xd

serverConfigVector    ParsingConfig::parsing(std::string fileName)
{
    fileToBeParsed.open(fileName);
    ServerConfig oneServerConfig = parsingOneServerConfig();
    addToVector(oneServerConfig);
    return (serverConfigs);
}

ServerConfig ParsingConfig::parsingOneServerConfig()
{
    ServerConfig oneServerConfig;
    oneServerConfig._port = 343;
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
