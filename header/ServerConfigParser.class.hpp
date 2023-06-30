#ifndef PARSINGCONFIG_HPP
# define PARSINGCONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.class.hpp"

typedef std::vector<ServerConfig> serverConfigVector;
typedef struct sockaddr_in				t_sockaddr_in;

class ServerConfigParser
{
	private:
		std::ifstream			_fileToBeParsed;
		serverConfigVector		_serverConfigs;
		StringBoolMap			_availableOptions;

	private:
		bool					checkForServerDeclaration();
		std::string				getRouteName(std::string &firstLine);
		bool					checkBooleanString(std::string value);
		void 					addAllowedMethod(std::string method, int &allowedMethod);
		void					addErrorPage(ServerConfig &oneServerConfig, std::string line);
		ServerConfig			parsingOneServerConfig();
		void					initializeConfiguration(ServerConfig &oneServerConfig, std::string line);
		void					initializeRoute(std::string line, ServerConfig::route newRoute);
		ServerConfig::route		addRoute(std::string firstLine, ServerConfig &oneServerConfig);
		t_sockaddr_in   		setAddress(int port);
		void					addToVector(ServerConfig &oneServerConfig);
		void					removeCommentFrom(std::string &line);
		std::string				trim(std::string str);
        std::string 			validateTrim(std::string str, int (*checkFunc)(int));
        void					extractKeyValue(std::string line, std::string &key, std::string &value);
		template <typename Key, typename Value>
		void					printMap(const char *colour_header, const char *colour_body, \
																	 std::map<Key, Value> &map);

	public:
		serverConfigVector		getServerConfigs();
	
	public:
		ServerConfigParser(std::string fileName);
		~ServerConfigParser(void);
};

#endif