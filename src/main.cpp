#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "utils.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "Error: Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    std::string strPort = av[1];
    std::string password = av[2];
    if (strContainOnlyDigits(strPort) == false) {
        std::cerr << "Error: Port not contain only digits" << std::endl;
        return 1;
    }
    int port = std::atoi(strPort.c_str());
    Server server(port, password);
    server.listening();
    return 0;
}
