#include "../inc/irc.hpp"

Server server;

void handleSignal(int signal) {
    if (signal == SIGINT) {
        for (size_t i = 0; i < server.getClientFDs().size(); i++) {
            int socket = server.getClientFDs()[i];
            server.sendData(socket, "Server closed\r\n");
        }
        close(server.getSocket());
        exit(1);
    }
}

int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "ERROR :Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    std::string strPort = av[1];
    std::string password = av[2];
    if (strContainOnlyDigits(strPort) == false) {
        std::cerr << "ERROR :Port not contain only digits" << std::endl;
        return 1;
    }
    int port = std::atoi(strPort.c_str());
    server = Server(port, password);
    try {
        signal(SIGINT, handleSignal);
        server.start();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
