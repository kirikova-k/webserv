#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>

namespace ft {
    class Server {
        public://priavate??
        
        //хз какие типы должны быть
        std::string host;
        int port;
        std::string server_name;
        int maxBodySize;
        bool autoindex;
        // std::vector<Location> locations;

        Server();
        ~Server();
        Server &operator=(const Server &rhs);

    };
}

#endif
