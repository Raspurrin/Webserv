#ifndef SERVERCONFIG_CLASS_HPP
# define SERVERCONFIG_CLASS_HPP

#include "webserv.hpp"

#define GET 1
#define POST 2
#define DELETE 4

typedef struct pollfd					t_pollfd;

class ServerConfig
{
	private:
		typedef struct route
		{
			const int			methods;
			const bool			dir_list;
			const std::string	HTTP_redirect;
			const std::string	root;
			const std::string	index;
		}	t_route;

		const int									_port;
		std::string									_name;
		const int									_clientBodySize;
		std::map<std::string, std::string>			_errorPages;
		std::map<const std::string, const t_route>	_routes;
	
	public:
		int					getPort() const;
		bool				checkRoutePath(const std::string &path) const;
		int					getMethods(const std::string &path) const;
		bool				isMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isDirListEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string	getRouteIndex(const std::string &path) const;
		struct sockaddr		getServerAddress();
		int					getServerSocketFd();
		
	public:
		ServerConfig(ParsingConfig builder);
		ServerConfig&	operator=(ServerConfig const &assign);
		~ServerConfig(void);
};

#endif
