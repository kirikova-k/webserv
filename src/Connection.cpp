#include "../inc/Connection.hpp"
// #include <sstream>


Connection::Connection() {}

Connection::Connection(int fd, int listen_fd) : fd(fd), listen_fd(listen_fd), status(READ), position(0) {
	std::cout << "listen: " << listen_fd << " ; for answer: " << fd << std::endl;
}

Connection::~Connection() {}


Connection::Connection(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->position = other.position;
	this->status = other.status;
}

Connection & Connection::operator=(Connection const & other)
{
	this->fd = other.fd;
	this->request = other.request;
	this->response = other.response;
	this->position = other.position;
	this->status = other.status;
	return *this;
}

int Connection::getFd() const { return this->fd; }

Request & Connection::getRequest() {/* std::cout << "in getter url\n " << this->request.getUrl(); */return this->request; }

Response & Connection::getResponse() { return this->response; }

int Connection::getPosition() const { return this->position; }

ConStatus Connection::getStatus() { return this->status; }


void Connection::setRequest(Request request) { this->request = request; }

void Connection::setResponse(Response response) { this->response = response; }

void Connection::setPosition(int position) { this->position = position; }

void Connection::setStatus(ConStatus status) { this->status = status; }


void Connection::readRequest(int listen_fd)
{
	char buf[BUFFER_SIZE];
	// std::cout << "\nPORT " << getServers()[i].getPort() << std::endl; 
	std::cout << "fd before recv " << listen_fd << std::endl;
	size_t bytes_read = recv(listen_fd, buf, sizeof(buf), 0);
	printf("read %zu bytes\n", bytes_read);	
	if (bytes_read == 0)
	{
		std::cout << "Can't receive client's request" << std::endl;
	}
	buf[bytes_read] = '\0';
	std::string requestData = buf;
	
	request = Request(requestData);
	status = READ_DONE;
	std::cout << "Read ok, url in request " << request.getUrl() << " status " << status << std::endl;
}

int Connection::sendHeaders()
{
	std::cout << "send headers" << std::endl;
	if (send(fd, response.toString().c_str(), response.toString().length(), 0) < 0)
		return -1;
	return 0;
}

int Connection::sendBody()
{
	
	if (response.getBodyFile() != "")
	{
		std::cout << "send file" << std::endl;
		FILE* file = fopen(response.getBodyFile().c_str(), "rb");
		fseek(file, position, SEEK_SET);
		
		char buffer[BUFFER_SIZE];
		int bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file);
		position += bytes_read;
		if (send(fd, buffer, bytes_read, 0) < 0)
			return -1;
		fclose(file);
		std::cout << "file sent ok: " << bytes_read << " bytes, position " << position 
			<< " length " << response.getLength() << std::endl;
	}
	if (position == response.getLength() || response.getBodyFile() == "")
	{
		std::string delimeter = "\r\n\r\n";
		if (send(fd, delimeter.c_str(), delimeter.length(), 0) < 0)
			return -1;
		status = WRITE_DONE;
	}
	else
		status = WRITE;
	return 0;
}