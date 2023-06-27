#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.class.hpp"

typedef std::vector<ServerConfig> serverConfigVector;
typedef struct sockaddr_in				t_sockaddr_in;

class ParsingConfig
{
	private:
		std::ifstream			fileToBeParsed;
		serverConfigVector		serverConfigs;

	private:
		ServerConfig			parsingOneServerConfig();
		ServerConfig::t_route	addRoute();
		t_sockaddr_in   		setAddress(int port);
		void					addToVector(ServerConfig &oneServerConfig);

	public:
		serverConfigVector		parsing(std::string fileName);
	
	public:
		ParsingConfig(void);
		~ParsingConfig(void);
};

#endif