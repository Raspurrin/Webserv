#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.class.hpp"

class ParsingConfig
{
    private:
        typedef struct route
        {
            int				methods;
            bool			dir_list;
            std::string		HTTP_redirect;
            std::string		root;
            std::string		index;
        }	t_route;
    
        int							        _port;
        std::string							_name;
        int							        _clientBodySize;
        std::map<std::string, std::string>	_errorPages;
        std::map<std::string, t_route>	    _routes;

    public:
        ParsingConfig   parsing(int fd);
        void            builder();
    
    public:
        ParsingConfig(void);
        ~ParsingConfig(void);
};

#endif