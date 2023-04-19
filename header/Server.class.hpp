#ifndef Server_CLASS_HPP
# define Server_CLASS_HPP

#include "../header/webserv.hpp"
#include "../header/colours.hpp"

class Server
{
	private: 
		int				opt, serverSocket, addressLen;
		const int							port;
		struct sockaddr_in					address;
		std::vector<struct pollfd>			pollFds;
		std::string							response;
		std::map<std::string, std::string>	configMap;
 
	private:
		std::string	trim(std::string);
		void		getRequest(int fd);
		void		postResponse(int socket, int indexToRemove);
		void		configureSocket(int newSocket);
		void		addNewConnection(int newSocket);
		void		parseConfFile(std::string confFileName);
		void		configureServer(void);

	public:
		void	checkConnections(void);

	public:
		Server(void);
		//Socket(Socket const &src);
		//Server&	operator=(Server const &assign);
		~Server(void);
};

#endif
