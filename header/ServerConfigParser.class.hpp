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
		void					setCGI(std::string &value, route &newRoute);
		void 					setHost(std::string &value, ServerConfig &oneServerConfig);
		void 					checkForServerNameDuplicate(const std::string &serverName) const;
		void 					checkMinimumConfiguration(ServerConfig &oneServerConfig);
		bool 					validate(std::string str, int (*checkFunc)(int));
		void 					setRouteHTTPRedirect(std::string &value, route &newRoute);
		std::string				findFirstWord(std::string line);
		void 					setRouteRoot(std::string &root, route &newRoute);
		void 					setRouteIndex(std::string &index, route &newRoute);
		void					setPort(std::string &value, ServerConfig &oneServerConfig);
		void					setServerName(std::string &serverName, ServerConfig &oneServerConfig);
		void					setClientBodySize(std::string &value, ServerConfig &oneServerConfig);
		bool					checkForServerDeclaration();
		std::string				getRouteName(std::string &firstLine);
		bool					checkBooleanString(std::string value);
		void 					addAllowedMethod(std::string method, int &allowedMethod);
		void					addErrorPage(ServerConfig &oneServerConfig, std::string line);
		ServerConfig			parsingOneServerConfig();
		void					initializeConfiguration(ServerConfig &oneServerConfig, std::string line);
		void					initializeRoute(std::string line, route &newRoute);
		route		addRoute(std::string firstLine, ServerConfig &oneServerConfig);
		void			   		setAddress(ServerConfig &oneServerConfig);
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
		ServerConfigParser(const char *fileName);
		~ServerConfigParser(void);
};

#endif