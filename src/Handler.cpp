#include "../inc/Handler.hpp"
#include "../inc/Request.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <fcntl.h>

// Handler::Handler() {}

Handler::Handler(Request & req, ft::Server & server) : request(req), server(server)
{
	if (request.getType() != HEADERS)
		saveFile();
	else if (request.getMethod() == "GET")
		methodGet();
	else if (request.getMethod() == "POST")
		methodPost();
	else if (request.getMethod() == "DELETE")
		methodDelete();
	
}

Handler::~Handler() {}

Response Handler::getResponse()
{
	return this->response;
}

void Handler::methodGet()
{
	this->returnFile();
}

void Handler::methodPost()
{
	std::ofstream file(request.getUrl() + ".txt");
	std::map<std::string, std::string>::iterator it;
	for (it = request.getBodyPOST().begin(); it != request.getBodyPOST().end(); it++)
	{
		file << it->first
				<< ':'
				<< it->second 
				<< std::endl;
	}
	this->returnFile();
}

void Handler::saveFile() // добавить exception
{
	std::cout << "saving file " << request.getFilename() << std::endl;
	std::string upload = server.getUploadPath().substr(1, server.getUploadPath().size() - 1);
	mkdir(upload.c_str(), 0777);
	std::string filename = upload + request.getFilename();
	
	FILE *file;
	if (request.getType() == DATA_START)
		file = fopen(filename.c_str(), "w");
	else
		file = fopen(filename.c_str(), "a");
	fwrite(request.getBody(), sizeof(char), request.getSize(), file);
	fclose(file);
}

void Handler::methodDelete()
{

}

void Handler::returnFile()
{
	std::string url = server.getRoot() + request.getUrl();
	if (url == server.getRoot())
		url = url + server.getIndex();
	const char *file_path = url.c_str();
	FILE* file = fopen(file_path, "rb");
	if (file == NULL)
	{
		std::cout << "Can't open file " << url << std::endl;
		response.setStatusCode(404);
		response.setContentLength(0);
		response.setBodyFile("");
	}
	else
	{
		std::cout << "File opened OK, URL: " << url << std::endl;
		response.setStatusCode(200);
		
		fseek(file, 0L, SEEK_END);
		int size = ftell(file);

		response.setContentLength(size);
		response.setBodyFile(url);
		
		fclose(file);
	}
	response.setHttpVersion(request.getHttp());
	response.setHeaders("Version: HTTP/1.1\r\nContent-Type: " + contentType() + "; charset=utf-8");

	// std::cout << "*** Response ***\n" << response.toString() << "\n***\n";
}

std::string Handler::contentType()
{
	std::string file = response.getBodyFile();
	int dot = file.find(".");
	std::string extension = file.substr(dot + 1, file.length() - dot);
	return response.getContentType(extension);
}
