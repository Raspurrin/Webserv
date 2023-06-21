#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.class.hpp"

class ParsingConfig
{
    public:
        std::vector<ServerConfig> parsing(int fd);
    
    public:
        ParsingConfig(void);
        ~ParsingConfig(void);
};

#endif