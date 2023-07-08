#include "../header/Cgi.class.hpp"
#include "../header/error.h"
#include "../header/utils.hpp"
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
}

static StringStringMap parse_output(const std::string &output) {
	StringStringMap response;
	size_t header_end = output.find("\r\n\r\n");
	std::string line;

	if (header_end == std::string::npos) {
		throw ErrC(Internal_Error);
	}

	std::istringstream in(output.substr(0, header_end));
	while (getline(in, line)) {
		if (line.size() == 1)
			break;

		size_t separator = line.find(':');
		if (separator == std::string::npos || line[separator + 1] != ' ') {
			throw ErrC(Internal_Error);
		}
		std::string key = line.substr(0, separator);
		std::string value = line.substr(separator + 2);
		response[key] = value;
	}
	response["Version"] = "HTTP/1.1";
	response["Status code"] = "200 OK";

	response["Body"] = output.substr(header_end + 4);
	return response;
}

static void check_output(StringStringMap &output) {
	if (output.find("Content-Type") == output.end()) {
		throw ErrC(Internal_Error);
	}

	if (output.find("Content-Length") == output.end()) {
		output["Content-Length"] = lenToStr(output["Body"]);
	}

}

StringStringMap Cgi::runGet() {
	//FIXME: Replace with a safe implementation
	std::string tmp_file = std::tmpnam(NULL);
	int pid = fork();
	if (pid == 0) {
		std::vector<const char*> env;
		for (ArgVec::iterator it = _env.begin(); it < _env.end(); it++) {
			env.push_back((*it).c_str());
		}
		env.push_back(NULL);
		char *argv[2];
		argv[1] = 0;
		std::string& path = _headerFields["Path"];
		std::string cgi_file_path = path.substr(1, path.find('?'));
		argv[0] = const_cast<char *>(cgi_file_path.c_str());
		std::cout << cgi_file_path << std::endl;
		int tmp_fd = open(tmp_file.c_str(), O_CREAT);
		std::cout << "tmp_fd: " << tmp_fd << std::endl;
		int ret = dup2(tmp_fd, STDOUT_FILENO);
		if (ret == -1)
			perror("Dup2: ");
		execve(argv[0], argv, const_cast<char* const*>(env.data()));
		perror("Execve failed: ");
		exit(1);
	} else {
		int i = 0;
		waitpid(pid, &i, 0);
		if (!WIFEXITED(i) || WEXITSTATUS(i) != EXIT_SUCCESS) {
			throw ErrC(Internal_Error);
		}
		std::ifstream output_file(tmp_file.c_str());
		if (output_file.bad()) {
			std::cout << "Fd is bad.\n";
			throw ErrC(Internal_Error);
		}
		std::ostringstream output;
		output << output_file.rdbuf();

		StringStringMap response = parse_output(output.str());
		check_output(response);
		return response;
	}
}