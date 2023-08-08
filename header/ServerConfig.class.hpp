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
		friend class ServerConfigParser;
		
		typedef struct myRoute
		{
			int				_methods;
			bool			_directoryListing;
			std::string		_HTTPRedirect;
			std::string		_root;
			std::string		_index;
			std::set<std::string> _CGI;
			myRoute() : _methods(0), _directoryListing(false), _HTTPRedirect(""), _root(""), _index("") {};
		} route;

		typedef struct pollfd						t_pollfd;
		typedef std::map<std::string, route> 		StringRouteMap;

		int					_host;
		int					_port;
		std::string			_name;
		int					_clientBodySize;
		StringStringMap		_errorPages;
		StringRouteMap		_routes;
		t_sockaddr_in		_address;
	
	private:
		size_t				calculateStringOverlap(const std::string &str1, const std::string &str2) const;

	public:
		// bool				setClientBodySize(int &clientBodySize);
		std::string			getHTTPRedirect(const std::string &path) const;
		bool 				setServerName(std::string &serverName);
		bool 				setPort(int &port);
		int					getPort() const;
		std::string			getName() const;
		int					getClientBodySize() const;
		std::string			getErrorPage(std::string errorCode) const;
		bool				isRouteValid(const std::string &path) const;
		int					getRouteMethods(const std::string &path) const;
		bool				isRouteMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isRouteDirListingEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string 	getRouteIndex(const std::string &path) const;
		t_sockaddr_in&		getAddress();
		void				printErrorPages() const;
		void				printServerConfig() const;

	public:
		ServerConfig();
		ServerConfig&	operator=(ServerConfig const &assign);
		~ServerConfig(void);
};

#endif
