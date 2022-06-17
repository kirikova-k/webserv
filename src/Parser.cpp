#include "Parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>

ft::Parser::Parser() {}

ft::Parser::Parser(const char *config) {
	_config = config;
}

ft::Parser::~Parser() {}
ft::Parser::Parser(const Parser &other) {
	 *this = other;
}

void ft::Parser::parse () {
	std::vector<std::string> config;
	size_t start = 0;
	size_t line = 0;
	size_t end = 0;

	config = checkAndClean(_config);
	checkAllKeys(config);
	while (line < config.size()) {
		if (!easyFind("server", config[line])) {
			start = line;
			if (checkBrackets(config, &line) < 0) {
				throw std::invalid_argument("Parser error: brackets");
			}
			end = line;
			_servers.push_back(Server());
			serversInfo(_servers.size() - 1, config, start, end);
		}
		else {
			throw std::invalid_argument("Parser error: server not found");
		}
		line++;
	}
	// config.clear();

//~~~~~~~PRINT~~~~~~~~~~//
for(int i=0; i < config.size(); i++){
   std::cout << "|" << config[i] << "|\n";
}
std::cout << std::endl;

for (int i=0; i < _servers.size(); i++) {
	std::cout << "Host: |" << _servers[i].getHost() << "|" << std::endl;
	std::cout << "Port: |" << _servers[i].getPort() << "|" << std::endl;
	std::cout << "Name: |" << _servers[i].getName() << "|" << std::endl;
}

//~~~~~~~PRINT~~~~~~~~~~//

}

void ft::Parser::serversInfo(size_t index, std::vector<std::string> file, size_t start, size_t end) {
	ft::ValidConfigKeys rootParams;
	std::string param;

	while (start < end) {
		for (size_t i = 0; i < rootParams.servParams.size(); i++) {
			if (!easyFind(rootParams.servParams[i], file[start]))
				fillConfig(rootParams.servParams[i], file[start], index, i);
			else if (!easyFind("location", file[start])) {
				// locationsInfo(file, index, &start, end);
				std::cout << "locations not ready yet!\n";
			}
		}
		start++;
	}
}

void ft::Parser::fillConfig(std::string key, std::string line, size_t index, size_t caseKey) {
	switch (caseKey) {
		case Host_port:
			fillHostPort(key, line, index);
			break;
		case Server_name:
			fillServerName(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Autoindex:
			// fillAutoindex(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Root:
			// fillServerRoot(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Index_page:
			// fillIndex(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Methods:
			// fillRootMethods(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Client_max_body_size:
			// fillRootMaxBodySize(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case UploadPath:
			// fillUploadPath(key, line, index);
			std::cout << "not ready yet!\n";
			break;
		case Error_page:
			// fillRootErrorPages(key, line, index);
			std::cout << "not ready yet!\n";
			break;
	}
}

void ft::Parser::fillServerName(std::string key, std::string line, ssize_t index) {
	std::vector<std::string> value;

	value = splitString(key, line);
	if (!_servers[index].getName().empty() or value.size() != 1)
		throw std::invalid_argument("Parser error: server name error");
	_servers[index].setName(value[0]);
}

void ft::Parser::fillHostPort(std::string key, std::string line, size_t index) {
	std::vector<std::string> value;

	value = splitString(key, line);
	if (_servers[index].getHost() != "" or _servers[index].getPort() or value.size() != 1)
		throw std::invalid_argument("Parser error: host/port error");
	value = checkHost(value[0]);
	if (value.size() > 1) {
		_servers[index].setHost(value[0]);
		_servers[index].setPort(checkPortVal(value[1]));
	}
	else {
		if (validHost(value[0]))
			_servers[index].setPort(checkPortVal(value[0]));
		else
			_servers[index].setPort(80);
	}
}

int ft::Parser::validHost(std::string value) {
	size_t pos = value.find(".");

	if (pos != std::string::npos)
		return 0;
	return 1;
}

int ft::Parser::checkPortVal (std::string str) {
	int val = 0;

	for (size_t i = 0; i < str.size(); i++) {
		if (!isdigit(str[i]))
			throw std::invalid_argument("Parser error: not digit");
	}
	val = static_cast<int>(strtod(str.c_str(), 0));
	if (val < 0 or val > 65535)
		throw std::invalid_argument("Parser error: number out of range");
	return val;
}

std::vector<std::string> ft::Parser::checkHost(std::string host) {
	size_t i = 0;
	size_t j = 0;
	size_t dot = 0;
	std::vector<std::string> hostPort;

	while(host[i]) {
		if (isdigit(host[i]))
			i++;
		else if (host[i] == '.') {
			if (j > 3)
				throw std::invalid_argument("Parser error: wrong format");
			j = 0;
			dot++;
			i++;
		}
		else if (host[i] == ':' and i) {
			hostPort.push_back(host.substr(0, i));
			hostPort.push_back(host.substr(i + 1, host.size()));
			return hostPort;
		}
		else
			throw std::invalid_argument("Parser error: wrong number");
		j++;
	}
	if (!hostPort.size())
		hostPort.push_back(host.substr(0, i));
	else
		throw std::invalid_argument("Parser error: wrong host/port");
	return hostPort;
}

std::vector<std::string> ft::Parser::splitString(std::string key, std::string line) {
	std::vector<std::string> value;
	size_t pos = 0;
	size_t i = 0;

	pos = (line.find(key) + key.size());
	while (line[pos] == ' ' or line[pos] == '\t') {
		pos++;
	}
	i = pos;
	while (i < line.size()) {
		pos = i;
		if (line[i] == ' ' or line[i] == '\t') {
			while (line[i] == ' ' or line[i] == '\t') {
				i++;
				pos++;
			}
		}
		else {
			while (line[i] and line[i] != ' ' and line[i] != '\t')
				i++;
			value.push_back(line.substr(pos, i - pos));
		}
	}
	return value;
}

int ft::Parser::checkBrackets(std::vector<std::string> file, size_t *line) {
	size_t openBracket = 0;
	size_t closeBracket = 0;
	size_t pos;

	pos = file[*line].find("server") + strlen("server");
	while(file[*line].size()) {
		while(file[*line][pos]) {
			if (file[*line][pos] == '{')
				openBracket++;
			else if (file[*line][pos] == '}')
				closeBracket++;
			if (!openBracket and file[*line][pos] != ' ' and
				file[*line][pos] != '\t' and file[*line][pos] != '\n')
				return -1;

			pos++;
		}
		pos = 0;
		(*line)++;
		if (!easyFind("server", file[*line])) {
			(*line)--;
			break ;
		}
	}
	if (openBracket != closeBracket)
		return -1;
	return 0;
}

std::vector<std::string>  ft::Parser::lineBreaks(std::string string) {
	std::vector<std::string> vec;
	std::string newString;
	std::string tmp;
	size_t i = 0;

	while (string[i]) {
		if (string[i] == '}' or string[i] == '{') {
			tmp = string.substr(i, string.size());
			newString = string.substr(0, i);
			cleanSpaces(&newString);
			if (!newString.empty())
				vec.push_back(newString);
			newString = tmp.substr(0, 1);
			vec.push_back(newString);
			newString = tmp.substr(1, tmp.size());
			cleanSpaces(&newString);
			if (!newString.empty())
				vec.push_back(newString);
			tmp.clear();
			newString.clear();
		}
		i++;
	}
	return vec;
}

int ft::Parser::isEmptyLine(std::string str) {
	size_t i = 0;

	while (str[i]) {
		if (str[i] != ' ' and str[i] != '\t' and str[i] != '\n')
			return -1;
		i++;
	}
	return 0;
}
void ft::Parser::cleanSpaces(std::string *string) {
	size_t i = 0;

	if ((*string)[i] == ' ' or (*string)[i] == '\t') {
		while ((*string)[i] == ' ' or (*string)[i] == '\t')
			i++;
	}
	*string = (*string).substr(i, (*string).size());
	i = (*string).size();
	if ((*string)[i - 1] == ' ' or (*string)[i - 1] == '\t' or (*string)[i - 1] == ';') {
		while ((*string)[i - 1] == ' ' or (*string)[i - 1] == '\t' or (*string)[i - 1] == ';')
			i--;
	}
	if (i < (*string).size())
		*string = (*string).substr(0, i);
}

std::vector<std::string> ft::Parser::checkAndClean(std::string argv) {
	std::ifstream file(argv);
	std::string buf;
	std::vector<std::string> tmp;
	std::vector<std::string> config;
	if (argv.substr((argv.size() - strlen(".conf")), strlen(".conf")) != ".conf" || (!file.is_open())) {
		throw std::invalid_argument("Parser error: wrong file type");
	}
	else {
		while (!file.eof()) {
			getline(file, buf);
			if (buf[0] == '#' || buf.empty()) {
				continue;
			}
			else if (buf.find('#')) {
				buf = buf.substr(0, buf.find('#'));
				if (!isEmptyLine(buf)) {
					continue ;
				}
			}
			tmp = lineBreaks(buf);
			if (!tmp.empty()) {
				for (size_t i = 0; i < tmp.size(); i++) {
					buf = tmp[i];
					tmp[i].clear();
					config.push_back(buf);
				}
			}
			else {
				cleanSpaces(&buf);
				config.push_back(buf);
			}
			buf.clear();
		}
	}
	config.push_back("");
	file.close();
	return config;
}

int ft::Parser::validKeys(std::string config) {

	std::vector<std::string>::iterator it;
	it = _validConfigParams.servKeys.begin();
	while (it != _validConfigParams.servKeys.end()) {
		if (*it == config) {
			return 0;
		}
		it++;
	}
	return -1;
}

int ft::Parser::easyFind(std::string word, std::string line) {
	size_t length = 0;
	size_t pos = -1;

	pos = line.find(word);
	if (pos != std::string::npos) {
		if (pos > 0) {
			for (size_t i = pos; line[i]; i--) {
				if (line[i - 1] == ' ' or line[i - 1] == '\t' or line[i - 1] == '\n')
					break ;
				length++;
			}
		}
		for (size_t i = pos; line[i]; i++) {
			if (line[i] == ' ' or line[i] == '\t' or line[i] == '\n')
				break ;
			length++;
		}
		if (length != word.size() or word != line.substr(pos, length))
			return -1;
	}
	else
		return -1;
	return 0;
}

void ft::Parser::checkAllKeys(std::vector<std::string> config) {
	std::vector<std::string>::iterator it;
	std::string newStr;
	size_t i = 0;

	it = config.begin();
	while (it < config.end()) {
		i = 0;
		while ((*it)[i] != ' ' and (*it)[i] != '\t' and (*it)[i])
			i++;
		newStr = (*it).substr(0, i);
		if (validKeys(newStr))
			throw std::invalid_argument("Parser error: incorrect key in the config");
		it++;
	}
}