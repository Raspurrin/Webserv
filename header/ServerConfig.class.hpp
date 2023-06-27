#ifndef SERVERCONFIG_CLASS_HPP
# define SERVERCONFIG_CLASS_HPP

#include "webserv.hpp"

#define GET 1
#define POST 2
#define DELETE 4

typedef struct sockaddr_in	t_sockaddr_in;

class ServerConfig
{
	private:
		friend class ParsingConfig;
		
		typedef struct route
		{
			int			_methods;
			bool		_directoryListing;
			std::string	_HTTPRedirect;
			std::string	_root;
			std::string	_index;
		}	t_route;

		typedef struct pollfd						t_pollfd;
		typedef std::map<std::string, t_route> 		StringRouteMap;

		int					_port;
		std::string			_name;
		int					_clientBodySize;
		StringStringMap		_errorPages;
		StringRouteMap		_routes;
		t_sockaddr_in		_address;
	
	public:
		int					getPort() const;
		bool				checkRoutePath(const std::string &path) const;
		int					getMethods(const std::string &path) const;
		bool				isMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isDirListEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string	getRouteIndex(const std::string &path) const;
		t_sockaddr_in&		getAddress();

	public:
		ServerConfig&	operator=(ServerConfig const &assign);
		~ServerConfig(void);
};

#endif
