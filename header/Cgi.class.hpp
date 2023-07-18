#pragma once

#include <vector>
#include <string>
#include <map>
#include "Client.class.hpp"

#define CGI_TIMEOUT_S 2

typedef std::vector<std::string>			ArgVec;

class Cgi {
private:
	StringStringMap	_headerFields;
	ArgVec			_env;
public:
	Cgi(StringStringMap&);

	StringStringMap runGet();
	StringStringMap runPost();
};