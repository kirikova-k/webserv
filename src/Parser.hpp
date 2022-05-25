#ifndef PARSER_HPP
#define PARSER_HPP
#include <vector>
#include "Server.hpp"
#include "ServerParameters.hpp"

enum ValidRootKeys {
	Host_port,
	Server_name,
	Autoindex,
	Root,
	Index_page,
	Methods,
	Client_max_body_size,
	Error_page,
	UploadPath,
	Redirection
};

enum ValidLocKeys {
	Location_name,
	Location_methods,
	Location_root,
	Location_redirection,
	Location_error_page,
	Bin_path_py,
	Bin_path_sh,
	Path_cgi,
	Location_index,
	LocationUploadPath,
	LocationAutoindex
};

namespace ft {
	class Parser {
		private:
		Parser();
		const char* _config;
		// std::vector<Port> _ports;
		std::vector<Server> _servers;

		public:
		Parser(const char* config);
		~Parser();

		ft::ValidConfigKeys _validConfigParams;

		void parse();
		std::vector<std::string> checkAndClean(std::string argv);
		int isEmptyLine(std::string str);
		void cleanSpaces(std::string *string);
		std::vector<std::string> lineBreaks(std::string string);
		int validKeys(std::string config);
		std::vector<std::string> checkHost(std::string host);
		void checkAllKeys(std::vector<std::string> config);
		int easyFind(std::string word, std::string line);
		int checkBrackets(std::vector<std::string> file, size_t *line);
	};
}


#endif