#include "../inc/Webserver.hpp"
#include "../inc/Request.hpp"
#include "../inc/Response.hpp"
#include "../inc/Handler.hpp"

// #define BUFFER_SIZE 1000

Webserver::Webserver() {}
// Webserver::Webserver(Config config) : config(config) {}
Webserver::Webserver(std::vector<ft::Server> &_servers) : servers(_servers) {}

Webserver::~Webserver() {}

void Webserver::prepare(int serv_id)
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(servers[serv_id].getPort());///
	addr_in.sin_len = sizeof(addr_in);
	addr_in.sin_addr.s_addr = inet_addr(servers[serv_id].getHost().c_str());///
	bzero(&(addr_in.sin_zero), 8);
	

	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0)
		err("socket");
	int on = 1;
	if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		err("setcockopt");
	if (bind(listen_socket, (struct sockaddr *) &addr_in, sizeof(struct sockaddr)) == -1)
		err("bind");
	if (listen(listen_socket, SOMAXCONN) == -1)
		err("listen");
	// memset(fds[serv_id], 0, sizeof(fds));
	fds[serv_id].fd = listen_socket;
	fds[serv_id].events = POLLIN;
	sockets.push_back(listen_socket);

	// return listen_socket;
}

void Webserver::run()
{
	for(int i = 0; i < servers.size(); i++) {
		servs_fd[fds[i].fd] = (getServers()[i]);
	}
	//~~~~~~~~~~~~~~~~~~~~~PRINT~~~~~~~~~~~~~~~~~~~~~~~~//
	std::map<int, ft::Server>::iterator it_begin = servs_fd.begin();
	while (it_begin != servs_fd.end()) {
		std::cout << "|" << it_begin->first << "|" << it_begin->second.getPort() << "|\n";
		++it_begin;
	}
	//~~~~~~~~~~~~~~~~~~~~~PRINT~~~~~~~~~~~~~~~~~~~~~~~~//
	
	
	end_server = false;
	nfds = getServers().size();
	std::cout << nfds << std::endl;
	while (end_server == false)
	{
		listenLoop();
	}
	for (int i = 0; i < nfds; i ++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}
}

void Webserver::listenLoop()
{
	int timeout = (3 * 60 * 1000);
	int new_sd = -1;
	std::cout << "--- wainting on poll" << std::endl;
	// std::cout << fds[0].fd << " " << fds[1].fd << " " << fds[2].fd << " fds qty " << nfds << std::endl;
	int rc = poll(fds, nfds, timeout);
	if (rc < 0)
		err("poll");
	if (rc == 0)
		err("poll timeout");
	int current_size = nfds;
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].revents == 0)
			continue;
		if (fds[i].revents != POLLIN && fds[i].revents != POLLOUT)
		{
			std::cout << "Error in revents " << fds[i].revents << std::endl;
			end_server = true;
			break;
		}
		if (std::find(sockets.begin(), sockets.end(), fds[i].fd) != sockets.end())
		{
			printf("Listening socket is readable\n");
			do
			{
				listen_fds = fds[i];
				int new_sd = accept(fds[i].fd, NULL, NULL);
				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						std::cerr << "accept error" << std::endl;
						end_server = true;
					}
					break;
				}
				std::cout << "new incoming connection " << new_sd << std::endl;
				fds[nfds].fd = new_sd;
				fds[nfds].events = POLLOUT;

				// connections[nfds] = new Connection(nfds, fds[i].fd);

				nfds++;
			} while (new_sd != -1);
		}
		else
		{
			printf("Descriptor %d is readable, listen fd %d\n", fds[i].fd, listen_fds.fd);
			if (sendAndReceive(fds[i], listen_fds) == -1)
			{
				std::cout << "Send error" << std::endl;
				closeConnection(i);
			}
		}
	}
}

int Webserver::sendAndReceive(struct pollfd fds, struct pollfd listen_fds)
{
	std::cout << "Send and receive, fd " << fds.fd << std::endl;
	if (connections.find(fds.fd) == connections.end())
			connections[fds.fd] = new Connection(fds.fd, listen_fds.fd);

	if (listen_fds.revents == POLLIN)
	{

		connections[fds.fd]->readRequest(listen_fds.fd);
		Handler handler(connections[fds.fd]->getRequest(), servs_fd[listen_fds.fd]); // add root dir setup
		connections[fds.fd]->setResponse(handler.getResponse());
	}

	if (fds.revents == POLLOUT)
	{
		if (connections[fds.fd]->sendHeaders() < 0)
			return -1;
		if (connections[fds.fd]->sendBody() < 0)
			return -1;
		std::cout << "DELETE CONNECTION\n";
		connections.erase(fds.fd);
	}


	// if (con.getStatus() == READ)
	// {
	// 	std::cout << "revents in0 " << fds.revents << std::endl;
	// 	con.readRequest(listen_fd);
	// }
	// if (con.getStatus() == READ_DONE)
	// {
	// 	std::cout << "revents in1 " << fds.revents << std::endl;
	// 	// fds.revents = 0;
	// 	// std::cout << "listen fd " << listen_fd << std::endl;
	// 	// std::cout << "get request " << con.getRequest().getUrl() << std::endl;
	// 	Handler handler(con.getRequest(), servs_fd[listen_fd]); // add root dir setup
	// 	con.setResponse(handler.getResponse());
	// 	if (con.sendHeaders() < 0)
	// 		return -1;
	// 	con.setStatus(WRITE);
	// }
	// if (con.getStatus() == WRITE)
	// {
	// 	std::cout << "revents in2 " << fds.revents << std::endl;
	// 	// fds.revents = 0;
	// 	if (con.sendBody() < 0)
	// 		return -1;
	// }
	// if (con.getStatus() == CLOSE)
	// {
	// 	closeConnection(fds.fd);
	// 	connections.erase(fds.fd);
	// }
	// if (con.getStatus() == WRITE_DONE)
	// {
	// 	std::cout << "DELETE CONNECTION\n";
	// 	connections.erase(fds.fd);
	// }
	return 0;
}


void Webserver::closeConnection(int i)
{
	close(fds[i].fd);
	fds[i].fd = -1;
	for (int i = 0; i < nfds; i++)
	{
		if (fds[i].fd == -1)
		{
			for (int j = 0; j < nfds - 1; j++)
				fds[i].fd = fds[j + 1].fd;
			i--;
			nfds--;
		}
	}
}

int Webserver::err(std::string msg) // добавить код возврата 500-какой-то в случае такой ошибки
{
	std::cerr << "Error " << errno << " in " << msg << std::endl;
	close(this->listen_socket);
	exit(errno);
}

const std::vector<ft::Server> &Webserver::getServers() {
	return servers;
}
