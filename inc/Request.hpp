#pragma once
#include <iostream>
#include <map>
#include <sys/stat.h>

// #include "Config.hpp"
#include "Server.hpp"

// #define HOME_DIR "res/"

class Request
{
	public:
		Request();
		// Request(std::string rawData, Config config);
		Request(std::string rawData);
		Request(Request const & other);
		~Request();

		Request operator=(Request const & other);
		
		std::string const getMethod() const;
		std::string const getUrl() const;
		std::string const getHttp() const;
		std::string const getBoundary() const;
		int const getDataType() const;
		std::string const getFilename() const;
		std::string const getBody() const;

		std::map<std::string, std::string> &getBodyPOST();

		void setUrl(std::string url);

		int check();
		
	private:
		std::string rawData;
		// Config config;
		ft::Server server;
		
		std::string method;
		std::string url;
		std::string httpVersion;
		std::map<std::string, std::string> headers;
		std::map<std::string, std::string> bodyPOST; // post method
		std::string body;
		std::string filename;
		int	dataType;
		// std::string contentType;
		std::string boundary;
		// int contentLength;
		
		
		void parseRequest(std::string rawData);
		void parseUrlencoded(std::string rawData, int start);
		void parseMultipart(std::string rawData, int start);
};
