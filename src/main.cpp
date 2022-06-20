#include <iostream>
#include "Parser.hpp"
#include "Server.hpp"


int main(int ac, char **av) {
    if (ac != 2){
        std::cout << "Error set configuration file" << std::endl;
        return 0;
    }
    ft::Parser parser(av[1]);
    try
    {
        parser.parse();
    }
   	catch (std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
	}
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}