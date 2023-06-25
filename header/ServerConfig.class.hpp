#ifndef SERVERCONFIG_CLASS_HPP
# define SERVERCONFIG_CLASS_HPP

#include "webserv.hpp"

#define GET 1
#define POST 2
#define DELETE 4


class ServerConfig
{
	private:
		friend class ParsingConfig;
		
		typedef struct route
		{
			int			methods;
			bool		directoryListing;
			std::string	HTTP_redirect;
			std::string	root;
			std::string	index;
		}	t_route;

		typedef struct pollfd						t_pollfd;
		typedef std::map<std::string, std::string>	StringStringMap;
		typedef std::map<std::string, t_route> 		StringRouteMap;

		int					_port;
		std::string			_name;
		int					_clientBodySize;
		StringStringMap		_errorPages;
		StringRouteMap		_routes;
	
	public:
		int					getPort() const;
		bool				checkRoutePath(const std::string &path) const;
		int					getMethods(const std::string &path) const;
		bool				isMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isDirListEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string	getRouteIndex(const std::string &path) const;

	public:
		ServerConfig&	operator=(ServerConfig const &assign);
		~ServerConfig(void);
};

#endif
