#ifndef ROUTE_CLASS_HPP
# define ROUTE_CLASS_HPP

#define GET 1
#define POST 2
#define DELETE 4

#include "webserv.hpp"
#include "../header/parsing.hpp"

class Route
{
	protected:
		std::string	name;
		int			methods;
		bool		directoryListening;
		// could contain: root, uploadDir and index:
		std::map<std::string, std::string> paths;
	
	private:
		Route(void);

	public:
		Route(std::string extractedRoute, std::string firstLine);
		//Route(Route const &src);
		//Route&	operator=(Route const &assign);
		~Route(void);
};

#endif
