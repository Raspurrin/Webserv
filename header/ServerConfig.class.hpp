#ifndef SERVERCONFIG_CLASS_HPP
# define SERVERCONFIG_CLASS_HPP

#include "webserv.hpp"

#define GET 1
#define POST 2
#define DELETE 4

class ServerConfig
{
	typedef struct route
	{
		const int											methods;
		const bool											dir_list;
		const std::string									HTTP_redirect;
		const std::string									root;
		const std::string									index;
	}	t_route;

	private:
		std::string											name;
		const int											clientBodySize;
		const struct sockaddr_in							address;
		const std::map<std::string, std::string>			errorPages;
		const std::map<const std::string, const t_route>	routes;

	private:
		

	public:
		bool				checkRoutePath(const std::string &path) const;
		int					getMethods(const std::string &path) const;
		bool				isMethodAllowed(const std::string &path, const int methodToCheck) const;
		bool				isDirListEnabled(const std::string &path) const;
		const std::string	getRouteRoot(const std::string &path) const;
		const std::string	getRouteIndex(const std::string &path) const;

	public:
		ServerConfig(void);
		//ServerManager(ServerManager const &src);
		//ServerManager&	operator=(ServerManager const &assign);
		~ServerConfig(void);
};

#endif
