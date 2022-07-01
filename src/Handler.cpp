#include "../inc/Handler.hpp"
#include "../inc/Request.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

#include <fcntl.h>

// Handler::Handler() {}

Handler::Handler(Request & req, ft::Server & server) : request(req), server(server)
{
	// std::cout << "Handler constructor\n ";// << request.getMethod() << std::endl;
	// if (request.check())
	// {
		if (request.getMethod() == "GET")
			methodGet();
		else if (request.getMethod() == "POST")
			methodPost();
		else if (request.getMethod() == "DELETE")
			methodDelete();
		else if (request.getDataType())
			saveFile();
	// }
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
	std::cout << "hello I'm POST meth" << std::endl;
	// std::ofstream file(request.getUrl() + ".txt");
	// // std::cout << request.getBodyPOST().begin()->first << std::endl;

	// std::map<std::string, std::string>::iterator it;
	// for (it = request.getBodyPOST().begin(); it != request.getBodyPOST().end(); it++)
	// {
	// 	// std::cout << it->first << " - " << it->second << std::endl;
	// 	file << it->first
	// 			<< ':'
	// 			<< it->second 
	// 			<< std::endl;
	// }
	this->returnFile();
}


void Handler::saveFile() // добавить exception
{
	
	std::string upload = server.getUploadPath().substr(1, server.getUploadPath().size() - 1);
	
	
	
	mkdir(upload.c_str(), 0777);
	std::string filename = upload + request.getFilename();

	std::cout << filename << std::endl;

	FILE *file = fopen(filename.c_str(), "a");

	int end = request.getBody().find("------WebKitFormBoundary");
	fwrite(request.getBody().c_str(),sizeof(char), end - 1, file);
	fclose(file);
}


void Handler::methodDelete()
{

}

void Handler::returnFile()
{
	// std::cout << "root in Handler " << server.getRoot() << std::endl;
	std::string url = server.getRoot() + request.getUrl();
	// std::cout << "URL in handler: " << url << std::endl;
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
	response.setHeaders("Version: HTTP/1.1\r\nContent-Type: " + contentType());

	std::cout << "*** Response ***\n" << response.toString() << "\n***\n";
}

std::string Handler::contentType()
{
	std::string file = response.getBodyFile();
	int dot = file.find(".");
	std::string extension = file.substr(dot + 1, file.length() - dot);
	return response.getContentType(extension);
}
