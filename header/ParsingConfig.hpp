#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"

class ParsingConfig
{
    public:
        std::vector<ServerConfig> parsing(int fd);
    
    public:
        ParsingConfig(void);
        ~ParsingConfig(void);
};

#endif