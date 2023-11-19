#include "Server.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) : port(port), password(password) {
    this->serverName = DEFAULT_SERVER_NAME;
    this->startTime = getCurrentTime();
    this->clientFds = NULL;
};

Server::~Server() {
    for (size_t i = 0; i < this->clients.size(); i++) {
        delete this->clients[i];
    }
    // for (size_t i = 0; i < this->channels.size(); i++) {
    //     delete this->channels[i];
    // }
    delete[] this->clientFds;
}

int Server::setNonBlocking(int serverSocket) {
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Error: Setting socket to non-blocking mode failed" << std::endl;
        return 1;
    }
    return 0;
}

void Server::setClientFds(void) {
    if (this->clientFds) {
        delete[] this->clientFds;
    }
    this->clientFds = new struct pollfd[this->clients.size() + 1];
    this->clientFds[0].fd = this->serverSocket;
    this->clientFds[0].events = POLLIN;
    for (size_t i = 0; i < this->clients.size(); i++) {
        this->clientFds[i + 1].fd = this->clients[i]->getFD();
        this->clientFds[i + 1].events = POLLIN;
    }
}

void Server::waitEvents(void) {
    int pollResult = poll(this->clientFds, this->clients.size() + 1, -1);
    if (pollResult > 0) {
        for (size_t i = 0; i < this->clients.size() + 1; i++) {
            // If revents == 0 no events occurred
            if (this->clientFds[i].revents == 0)
                continue;
            // Handle event on socket server
            if (i == 0)
                this->acceptConnection();
            // Handle event on socket clients
            else if (i > 0) {
                Client *client = this->clients[i - 1];
                this->receiveData(client);
            }
        }
    } else if (pollResult < 0) {
        std::cerr << "Error: Waiting connections failed" << std::endl;
    }
}

void Server::acceptConnection(void) {
    int clientSocket;
    do {
        struct sockaddr_in6 clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(this->serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            if (errno != EWOULDBLOCK) {
                std::cerr << "Error: Accepting connection failed" << std::endl;
            }
            break;
        }
        this->addClient(clientSocket, getClientIpAddress(&clientAddress), ntohs(clientAddress.sin6_port));
    } while (clientSocket != -1);
}

void Server::receiveData(Client *client) {
    char buffer[BUFFER_SIZE + 1];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(client->getFD(), buffer, sizeof(buffer), 0);
        if (bytesRead < 0) {
            if (errno != EWOULDBLOCK) {
                std::cerr << "Error: Receiving data from client fd: " << client->getFD() << " failed" << std::endl;
                this->delClient(client->getFD());
            }
            break;
        } else if (bytesRead == 0) {
            std::cout << "Client fd: " << client->getFD() << " disconnected" << std::endl;
            this->delClient(client->getFD());
            break;
        } else {
            buffer[bytesRead] = '\0';
            std::string buff = buffer;
            if (buff.at(buff.size() - 1) == '\n') {
                std::vector<std::string> cmds = splitString(client->getPartialRecv() + buff, '\n');
                client->setPartialRecv("");
                for (size_t i = 0; i < cmds.size(); i++) {
                    if (DEBUG) {
                        std::cout << "cmds[" << i << "] = " << cmds[i] << std::endl;
                    }
                    // this->handleCommand(cmds[i], client);
                }
            } else {
                client->setPartialRecv(client->getPartialRecv() + buff);
                if (DEBUG) {
                    std::cout << "Partial recv from client fd: " << client->getFD() << " = " << buff << std::endl;
                }
            }
        }
    }
}

void Server::addClient(int clientSocket, std::string clientIpAddress, int clientPort) {
    std::string ip = clientIpAddress;
    if (clientIpAddress.empty() || clientIpAddress == "1")
        ip = "127.0.0.1";
    this->clients.push_back(new Client(this, clientSocket, clientIpAddress, clientPort));
    this->setNonBlocking(clientSocket);
    this->setClientFds();
    if (DEBUG) {
        std::cout << "New connection fd: " << clientSocket << ", ip: " << clientIpAddress << ", port: " << clientPort << std::endl;
    }
}

void Server::delClient(int clientSocket) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i]->getFD() == clientSocket) {
            if (DEBUG) {
                std::cout << "Close connection fd: " << clientSocket << ", ip: " << this->clients[i]->getIP() << ", port: " << this->clients[i]->getPort() << std::endl;
            }
            this->clients.erase(this->clients.begin() + i);
            break;
        }
    }
    this->setClientFds();
    close(clientSocket);
}

void Server::handleCommand(std::string command, Client *client) {
    std::cout << "Handling " << command << " from  client fd: " << client->getFD() << std::endl;
}

int Server::listening(void) {
    this->serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (this->serverSocket < 0) {
        std::cerr << "Error: Creating server socket failed" << std::endl;
        return 1;
    }
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        std::cerr << "Error: Setting socket options failed" << std::endl;
        close(this->serverSocket);
        return 1;
    }
    if (this->setNonBlocking(this->serverSocket) == 1) {
        close(this->serverSocket);
        return 1;
    }
    // Initializing serverAddress
    struct sockaddr_in6 serverAddress;
    struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_addr = in6addr_any;
    serverAddress.sin6_port = htons(this->port);
    serverAddress.sin6_flowinfo = 0;
    serverAddress.sin6_scope_id = 0;
    if (bind(this->serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error: Binding socket to the port failed" << std::endl;
        close(this->serverSocket);
        return 1;
    }
    std::cout << "IRC Server started. Listening for connections on port " << this->port << " ..." << std::endl;
    if (listen(this->serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Error: Listening for connections failed" << std::endl;
        close(this->serverSocket);
        return 1;
    }
    std::cout << "Waiting for connections ..." << std::endl;
    this->setClientFds();
    while (true) {
        this->waitEvents();
    }
    return 0;
}
