#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"

class ParsingConfig
{
    std::vector<ServerConfig> parsing(int fd);
    ParsingConfig(void);
    ~ParsingConfig(void);
};

#endif