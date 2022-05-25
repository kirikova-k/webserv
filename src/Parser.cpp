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
			// serversInfo(_servers.size() - 1, config, start, end);
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
//~~~~~~~PRINT~~~~~~~~~~//

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
	if (_validConfigParams.servKeys.find(config)->first == config)
		return 0;
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