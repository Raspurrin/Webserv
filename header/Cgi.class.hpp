#pragma once

#include <vector>
#include <string>
#include <map>
#include "Client.class.hpp"

typedef std::vector<const char *>			ArgVec;
typedef std::map<std::string, std::string>	VarMap;

class Cgi {
private:
	Client&		_client;
	ArgVec	_env;
	VarMap		_var;
public:
	Cgi(Client &);
};