#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <vector>
#include <csignal>
#include <list>
#include <map>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <string.h>
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

#define REQUEST_TIMEOUT 2

typedef std::map<std::string, std::string>	StringStringMap;
typedef std::map<int, std::string>	IntStringMap;
typedef std::vector<int>			IntVector;
typedef struct statusResponse {
	int _code;
	std::string _description;
	std::string _message;
} t_status;

extern IntVector			_indexesToRemove;

void	error_handle(std::string type);
IntStringMap	createErrorMap();

#endif
