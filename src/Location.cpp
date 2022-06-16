#include "Location.hpp"

ft::Location::Location() {
    path = "";
	root = "";
    index = "";
	upload_path = "";
    autoindex = false;
    methods = std::vector<std::string>();
    errorPages = std::map<int, std::string>();
}

ft::Location &ft::Location::operator=(const Location &other) {
    path = other.path;
    root = other.root;
    index = other.index;
    upload_path = other.path;
    autoindex = other.autoindex;
    methods = other.methods;
    errorPages = other.errorPages;
    return *this;
}

ft::Location::~Location() {}

void ft::Location::setPath(const std::string &_path) {
    path = _path;
}
const std::string &ft::Location::getPath() const {
    return path;
}
void ft::Location::setRoot(const std::string &_root) {
    root = _root;
}
const std::string &ft::Location::getRoot() const {
    return root;
}
void ft::Location::setIndex(const std::string &_index) {
    index = _index;
}
const std::string &ft::Location::getIndex() const {
    return index;
}
void ft::Location::setUploadPath(const std::string &_uploadPath) {
    upload_path = _uploadPath;
}
const std::string &ft::Location::getUploadPath() const {
    return upload_path;
}
void ft::Location::setAutoIndex(const bool &_autoIndex) {
    autoindex = _autoIndex;
}
const bool &ft::Location::getAutoIndex() const {
    return autoindex;
}
void ft::Location::setMethods(const std::vector<std::string> &_methods) {
    methods = _methods;
}
const std::vector<std::string> &ft::Location::getMethods() const{
    return methods;
}

void ft::Location::setErrorPages(const std::map<int, std::string> &_errorPages) {
    errorPages = _errorPages;
}
const std::map<int, std::string> &ft::Location::getErrorPages() const {
    return errorPages;
}
