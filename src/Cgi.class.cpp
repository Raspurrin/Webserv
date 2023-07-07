#include "../header/Cgi.class.hpp"
#include "../header/error.h"
#include "sys/wait.h"
#include "sys/types.h"

Cgi::Cgi(StringStringMap &headerFields): _headerFields(headerFields) {
}

void Cgi::prepareCgi() {
	_env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	if (_headerFields["Method"] == "POST") {
		_env.push_back("CONTENT_LENGTH=" + _headerFields["Content-Length"]);
		_env.push_back("CONTENT_TYPE=" + _headerFields["Content-Type"]);
	}
	_env.push_back("REQUEST_METHOD=" + _headerFields["Method"]);
	std::string& host = _headerFields["Host"];
	size_t pos = host.find(':');
	if (pos != std::string::npos) {
		_env.push_back("SERVER_PORT=" + host.substr(pos + 1));
	} else {
		_env.push_back("SERVER_PORT=80");
	}
	_env.push_back("SERVER_NAME=" + host.substr(0, pos));
	std::string& path = _headerFields["Path"];
	_env.push_back("QUERY_STRING=" + path.substr(path.find('?') + 1));
	_env.push_back("SCRIPT_NAME=" + path.substr(0, path.find('?')));
	_env.push_back("SERVER_SOFTWARE=webserv");
	for (StringStringMap::iterator it = _headerFields.begin(); it != _headerFields.end(); it++) {
		_env.push_back("HTTP_" + (*it).first + "=" + (*it).second);
	}
	//TODO: investigate if REMOTE_* variables are necessary.
	//TODO: implement PATH_INFO and PATH_TRANSLATED. https://www.mnm-team.org/pub/Diplomarbeiten/haub95/HTML-Version/node144.html https://www.rfc-editor.org/rfc/rfc3875#section-4.1.2
	_env.push_back(NULL);
}

std::string Cgi::runGet() {
	//FIXME: Replace with a safe implementation
	std::string tmp_file = std::tmpnam(NULL);
	int pid = fork();
	if (pid == 0) {
		std::vector<const char*> env;
		for (ArgVec::iterator it = _env.begin(); it < _env.end(); it++) {
			env.push_back((*it).c_str());
		}
		char *argv[2];
		argv[1] = 0;
		std::string& path = _headerFields["Path"];
		std::string cgi_file_path = path.substr(1, path.find('?'));
		argv[0] = const_cast<char *>(cgi_file_path.c_str());
		std::cout << cgi_file_path << std::endl;
		int tmp_fd = open(tmp_file.c_str(), O_CREAT);
		dup2(tmp_fd, STDOUT_FILENO);
		execve(argv[0], argv, const_cast<char* const*>(env.data()));
		perror("Execve failed: ");
		exit(1);
	} else {
		int i = 0;
		waitpid(pid, &i, 0);
		if (WEXITSTATUS(i) != EXIT_SUCCESS) {
			throw ErrC(Internal_Error);
		}
		std::ifstream output_file(tmp_file.c_str());
		if (output_file.bad()) {
			throw ErrC(Internal_Error);
		}
		std::ostringstream output;
		output << output_file.rdbuf();
		return output.str();
	}
}