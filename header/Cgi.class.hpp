#pragma once

#include <vector>
#include <string>
#include <map>
#include "Client.class.hpp"

#define CGI_TIMEOUT_S 2

typedef std::vector<std::string>			ArgVec;
typedef std::vector<const char*>			EnvVec;

class Cgi {
private:
	StringStringMap	_headerFields;
	ArgVec			_env;

	EnvVec			prepareEnvVec();
public:
	Cgi(StringStringMap&);

	StringStringMap runGet();
	StringStringMap runPost();
};