#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.class.hpp"

typedef std::vector<ServerConfig> serverConfigVector;
typedef struct sockaddr_in				t_sockaddr_in;

class ParsingConfig
{
	private:
		std::ifstream			_fileToBeParsed;
		serverConfigVector		_serverConfigs;

	private:
		ServerConfig			parsingOneServerConfig();
		ServerConfig::t_route	addRoute();
		t_sockaddr_in   		setAddress(int port);
		void					addToVector(ServerConfig &oneServerConfig);

		std::string				trim(std::string str);
        std::string 			validateTrim(std::string str, int (*checkFunc)(int));
        void					extractKeyValue(std::string line, std::string &key, std::string &value);
		template <typename Key, typename Value>
		void					printMap(const char *colour_header, const char *colour_body, \
																	 std::map<Key, Value> &map);

	public:
		serverConfigVector		getServerConfigs();
	
	public:
		ParsingConfig(std::string fileName);
		~ParsingConfig(void);
};

#endif