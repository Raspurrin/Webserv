#pragma once

#include <vector>
#include <string>
#include <map>
#include "Client.class.hpp"

typedef std::vector<std::string>			ArgVec;

class Cgi {
private:
	Client&		_client;
	ArgVec		_env;
public:
	Cgi(Client &);

	void prepareCgi();
	std::string runGet();
};