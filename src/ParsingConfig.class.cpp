 #include "ParsingConfig.hpp"
 #include "ServerConfig.class.hpp"

 //hardcoded for testing xd
ParsingConfig    ParsingConfig::parsing(int fd)
{

    std::map<std::string, t_route> routemap1;
    t_route route1;
    route1.methods = 1;
    route1.dir_list = false;
    route1.HTTP_redirect = "hi";
    route1.index = "4";
    route1.root = "dsfs";
    _routes["lol"] = route1;

    routemap1["route1"] = route1;
    _port = 343;
    _name = "dsfsdf";
    _clientBodySize = 4343;
    _errorPages["505"] = "sdfsdfs";
    return (*this);
}

ParsingConfig::ParsingConfig(void)
{
}

ParsingConfig::~ParsingConfig(void)
{
}