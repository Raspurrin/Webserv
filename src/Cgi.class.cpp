#include "../header/Cgi.class.hpp"

Cgi::Cgi(Client &client): _client(client) {
	int pid = fork();
	if (pid == 0) {
		StringStringMap headerFields = _client.getHeaderFields();
		_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
		std::string& host = headerFields["Host"];
		size_t pos = host.find(':');
		if (pos != std::string::npos) {
			_env.push_back(("SERVER_PORT=" + host.substr(pos + 1)).c_str());
		} else {
			_env.push_back("SERVER_PORT=80");
		}
		_env.push_back(("SERVER_NAME=" + host.substr(0, pos)).c_str());
		std::string& path = headerFields["Path"];
		_env.push_back(("QUERY_STRING=" + path.substr(path.find('?') + 1)).c_str());
		_env.push_back(NULL);
		char *argv[2];
		argv[1] = 0;
		std::string cgi_file_path = path.substr(1, path.find('?'));
		argv[0] = const_cast<char *>(cgi_file_path.c_str());
		std::cout << cgi_file_path << std::endl;
		dup2(_client.getSocket(), STDOUT_FILENO);
		execve(argv[0], argv, const_cast<char* const*>(_env.data()));
		perror("Execve failed: ");
		exit(1);
	} else if (pid == -1) {
		std::cout << "Fork failed in Cgi constructor\n";
	}
}