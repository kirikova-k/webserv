#include "Server.hpp"
#include "Parser.hpp"

ft::Server::Server() {
	host = "";
	port = 0;
	server_name = "";
	maxBodySize = 0;
	autoindex = false;
}
ft::Server::~Server() {}

ft::Server &ft::Server::operator=(const Server &rhs) {
	this->host = rhs.host;
	this->port = rhs.port;
	this->server_name = rhs.server_name;
	this->maxBodySize = rhs.maxBodySize;
	this->autoindex = rhs.autoindex;
	return *this;
}