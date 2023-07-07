#pragma once

#include <vector>
#include <string>
#include <map>
#include "Client.class.hpp"

typedef std::vector<std::string>			ArgVec;

class Cgi {
private:
	StringStringMap	_headerFields;
	ArgVec			_env;
public:
	Cgi(StringStringMap&);

	void prepareCgi();
	std::string runGet();
};