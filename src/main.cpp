#include <iostream>
#include "Parser.hpp"


int main(int ac, char **av) {
    if (ac != 2){
        std::cout << "Error set configuration file" << std::endl;
        return 0;
    }
    ft::Parser parser(av[1]);
    parser.parse();

}