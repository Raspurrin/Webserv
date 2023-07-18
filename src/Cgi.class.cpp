#include "../header/Cgi.class.hpp"
#include "../header/error.h"
#include "../header/utils.hpp"
#include "sys/wait.h"
#include "time.h"
#include "sys/types.h"
#include <cstdio>
#include <cstdlib>

Cgi::Cgi(StringStringMap &headerFields): _headerFields(headerFields) {
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
	if (path.find('?') != std::string::npos) {
		_env.push_back("QUERY_STRING=" + path.substr(path.find('?') + 1));
	} else {
		_env.push_back("QUERY_STRING=");
	}
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
		throw ErrC(Internal_Error, "Cgi: has no header end.");
	}

	std::istringstream in(output.substr(0, header_end));
	while (getline(in, line)) {
		if (line.size() == 1)
			break;

		size_t separator = line.find(':');
		if (separator == std::string::npos || line[separator + 1] != ' ') {
			throw ErrC(Internal_Error, "Cgi: Key has no value");
		}
		std::string key = line.substr(0, separator) + ":";
		std::string value = line.substr(separator + 2);
		response[key] = value;
	}
	response["Version"] = "HTTP/1.1";

	response["Body"] = output.substr(header_end + 4);
	return response;
}

static void check_output(StringStringMap &output) {
	if (output.find("Content-Type:") == output.end()) {
		throw ErrC(Internal_Error, "Cgi: Content-Type is missing.");
	}

	if (output.find("Content-Length:") == output.end()) {
		output["Content-Length:"] = lenToStr(output["Body"]);
	} else if (output["Content-Length:"] != lenToStr(output["Body"])) {
		// enforce correct content lengthg
		throw ErrC(Internal_Error, "Content-Length provided by cgi does not match size of returned body");
	}

	if (output.find("Status:") != output.end()) {
		output["Status code"] = output["Status:"];
	} else {
		output["Status code"] = "200 OK";
	}
}

static std::string get_child_output(std::string &file, int pid) {int status = 0;
	time_t start = time(NULL);
	do {
		int ret = waitpid(pid, &status, WNOHANG);
		if (ret == -1) {
			throw ErrC(Internal_Error, "waitpid failed");
		} else if (ret != 0) {
			break;
		}
	} while (time(NULL) - start < CGI_TIMEOUT_S);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
		throw ErrC(Internal_Error, "Child process failed");
	}

	std::ifstream output_file(file.c_str());
	if (output_file.bad()) {
		throw ErrC(Internal_Error, "Fd is bad.");
	}

	std::ostringstream output;
	output << output_file.rdbuf();

	// if (std::remove(file.c_str())) {
	// 	std::cout << "Could not remove tmp file";
	// }

	return output.str();
}

static std::string generate_filename() {
	std::string tmp_file;
	// std::stringstream str;
	// str << rand();
	// str >> tmp_file;
	// tmp_file = "/tmp/webserv" + tmp_file;
	// std::cout << "Tmp file is: " << tmp_file << std::endl;
	tmp_file = "tmp";
	return tmp_file;
}

EnvVec Cgi::prepareEnvVec() {
	EnvVec env;
	for (ArgVec::iterator it = _env.begin(); it < _env.end(); it++) {
		env.push_back((*it).c_str());
	}
	env.push_back(NULL);
	return env;
}

StringStringMap Cgi::runGet() {
	std::string tmp_file = generate_filename();
	int pid = fork();
	if (pid == 0) {
		EnvVec env = prepareEnvVec();
		char *argv[2];
		argv[1] = NULL;
		std::string& path = _headerFields["Path"];
		std::string cgi_file_path = path.substr(1, path.find('?') - 1);
		argv[0] = const_cast<char *>(cgi_file_path.c_str());
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
		int tmp_fd = open(tmp_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
		int ret = dup2(tmp_fd, STDOUT_FILENO);
		if (ret == -1)
			perror("Dup2");
		execve(argv[0], argv, const_cast<char* const*>(env.data()));
		perror("Execve failed");
		exit(1);
	} else {
		std::string output = get_child_output(tmp_file, pid);
		StringStringMap response = parse_output(output);
		check_output(response);
		return response;
	}
}

StringStringMap Cgi::runPost() {
	std::string out_file = generate_filename();
	std::string in_file = out_file + "_in";

	{
		std::ofstream inputfile(in_file.c_str());
		inputfile << _headerFields["Body"];
	}

	int pid = fork();
	if (pid == 0) {
		EnvVec env = prepareEnvVec();
		char *argv[2];
		argv[1] = NULL;
		std::string& path = _headerFields["Path"];
		std::string cgi_file_path = path.substr(1, path.find('?') - 1);
		argv[0] = const_cast<char *>(cgi_file_path.c_str());
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
		int out_fd = open(out_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
		int in_fd = open(in_file.c_str(), O_RDONLY);
		int ret = dup2(out_fd, STDOUT_FILENO);
		ret |= dup2(in_fd, STDIN_FILENO);
		if (ret == -1)
			perror("Dup2");
		execve(argv[0], argv, const_cast<char* const*>(env.data()));
		perror("Execve failed");
		exit(1);
	} else {
		// if (std::remove(in_file.c_str())) {
		// 	std::cout << "Could not remove tmp file";
		// }
		std::string output = get_child_output(out_file, pid);
		StringStringMap response = parse_output(output);
		check_output(response);
		return response;
	}
}