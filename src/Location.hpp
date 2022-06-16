#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <map>

namespace ft {

	class Location {

	private:

		std::string path;
		std::string root;
		std::string index;
		std::string upload_path;
		bool autoindex;
		std::vector<std::string> methods;
		std::map<int, std::string> errorPages;

	public:

		Location();
		Location &operator=(const Location &other);
		~Location();

		void setPath(const std::string &_path);
		const std::string &getPath() const;

		void setRoot(const std::string &_root);
		const std::string &getRoot() const;

		void setIndex(const std::string &_index);
		const std::string &getIndex() const;

		void setUploadPath(const std::string &_uploadPath);
		const std::string &getUploadPath() const;

		void setAutoIndex(const bool &_autoIndex);
		const bool &getAutoIndex() const;

		void setMethods(const std::vector<std::string> &_methods);
		const std::vector<std::string> &getMethods() const;

		void setErrorPages(const std::map<int, std::string> &_errorPages);
		const std::map<int, std::string> &getErrorPages() const;

	};

}

#endif 
