#include "../header/Cgi.class.hpp"
#include "../header/Error.class.hpp"
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
	if (_headerFields.count("Query-String") > 0) {
		_env.push_back("QUERY_STRING=" + _headerFields["Query-String"]);
	} else {
		_env.push_back("QUERY_STRING=");
	}
	// FIXME: CHANGE THIS!!!!
	_env.push_back("SCRIPT_NAME=" + _headerFields["Filename"]);
	_env.push_back("SERVER_SOFTWARE=webserv");
	for (StringStringMap::iterator it = _headerFields.begin(); it != _headerFields.end(); it++) {
		_env.push_back("HTTP_" + (*it).first + "=" + (*it).second);
	}
	_env.push_back("PATH_TRANSLATED=" + _headerFields["Path_Info"]);
	_env.push_back("PATH_INFO=" + _headerFields["Path"]);
}

static StringStringMap parse_output(const std::string &output) {
	StringStringMap response;
	size_t header_end = output.find("\r\n\r\n");
	std::string line;

	if (header_end == std::string::npos) {
		throw ErrorResponse(500, "Cgi: has no header end.");
	}

	std::istringstream in(output.substr(0, header_end));
	while (getline(in, line)) {
		if (line.size() == 1)
			break;

		size_t separator = line.find(':');
		if (separator == std::string::npos || line[separator + 1] != ' ') {
			throw ErrorResponse(500, "Cgi: Key has no value");
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
		throw ErrorResponse(500, "Cgi: Content-Type is missing.");
	}

	if (output.find("Content-Length:") == output.end()) {
		output["Content-Length:"] = lenToStr(output["Body"]);
	} else if (output["Content-Length:"] != lenToStr(output["Body"])) {
		// enforce correct content lengthg
		throw ErrorResponse(500, "Content-Length provided by cgi does not match size of returned body");
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
			throw ErrorResponse(500, "waitpid failed");
		} else if (ret != 0) {
			break;
		}
	} while (time(NULL) - start < CGI_TIMEOUT_S);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
		throw ErrorResponse(500, "Child process failed");
	}

	std::ifstream output_file(file.c_str());
	if (output_file.bad()) {
		throw ErrorResponse(500, "Fd is bad.");
	}

	std::ostringstream output;
	output << output_file.rdbuf();

	if (std::remove(file.c_str())) {
		std::cout << "Could not remove tmp file";
	}

	return output.str();
}

static std::string generate_filename() {
	std::string tmp_file;
	std::stringstream str;
	str << rand();
	str >> tmp_file;
	tmp_file = "/tmp/webserv" + tmp_file;
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
		std::string output = get_child_output(out_file, pid);
		if (std::remove(in_file.c_str())) {
			std::cout << "Could not remove in tmp file";
		}
		StringStringMap response = parse_output(output);
		check_output(response);
		return response;
	}
}
