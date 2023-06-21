#ifndef SERVERCONFIG_CLASS_HPP
# define SERVERCONFIG_CLASS_HPP

#include "webserv.hpp"

#define GET 1
#define POST 2
#define DELETE 4

typedef struct route
{
	const int										methods;
	const bool										dir_list;
	const std::string								HTTP_redirect;
	const std::string								root;
	const std::string								index;

	route() = default;
}	t_route;

typedef struct pollfd					t_pollfd;

class ServerConfig
{

	//std::vector<std::stringstream>						orderedInput;
	const int											_port;
	//TODO: initialize port
	std::string											_name;
	const int											_clientBodySize;
	const std::map<std::string, std::string>			_errorPages;
	const std::map<const std::string, const t_route>	_routes;
	
	public:
		//template<typename T>
		// T				parsingKeyValue(std::stringstream Input);
		int					getPort() const; // TODO: make getport()
		bool				checkRoutePath(const std::string &path) const;
		int					getMethods(const std::string &path) const;
		bool				isMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isDirListEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string	getRouteIndex(const std::string &path) const;
		struct sockaddr		getServerAddress();
		int					getServerSocketFd();
		
	public:
		ServerConfig(const int port, const std::string& name, const int clientBodySize, \
                 	const std::map<std::string, std::string>& errorPages, \
                 	const std::map<const std::string, const t_route>& routes);
		ServerConfig(ServerConfig const &src);
		ServerConfig&	operator=(ServerConfig const &assign);
		~ServerConfig(void);
};

#endif
