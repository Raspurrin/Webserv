#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <vector>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "Request.class.hpp"

void	error_handle(std::string type);

#endif
