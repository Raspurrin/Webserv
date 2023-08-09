#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <bits/types/sig_atomic_t.h>
#include <vector>
#include <csignal>
#include <list>
#include <map>
#include <set>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
//#include "Request.class.hpp"
#include <netinet/tcp.h>

#ifndef DEBUG
# define DEBUG 0
#endif

#define REQUEST_TIMEOUT 10

typedef std::map<std::string, std::string>	StringStringMap;
typedef std::map<std::string, int>			StringIntMap;
typedef std::map<std::string, bool>         StringBoolMap;
typedef std::map<int, std::string>			IntStringMap;
typedef std::vector<int>					IntVector;

typedef struct statusResponse {
	int _code;
	std::string _description;
	std::string _message;
} t_status;

typedef struct myRoute
{
	int				_methods;
	bool			_directoryListing;
	std::string		_HTTPRedirect;
	std::string		_root;
	std::string		_index;
	std::set<std::string> _CGI;
	myRoute() : _methods(0), _directoryListing(false), _HTTPRedirect(""), _root(""), _index("") {};
} route;

typedef std::map<std::string, route> 		StringRouteMap;


extern IntVector				_indexesToRemove;
extern volatile sig_atomic_t	g_shutdown_flag;

void	error_handle(std::string type);
IntStringMap	createErrorMap();

#endif
