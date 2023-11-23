#include "../inc/irc.hpp"

Server::Server(){};
Server::Server(int port, std::string password) : port(port), password(password) {
    this->startTime = getCurrentTime();
};

std::vector<Client *> Server::getRegisteredClients(void) {
    return this->registeredClients;
};

std::vector<int> Server::getClientFDs(void) {
    return this->clientFDs;
}

int Server::getSocket(void) {
    return this->serverSocket;
}

std::string Server::getPassword(void) {
    return this->password;
}

Server::~Server() {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        delete this->registeredClients[i];
    }
    for (size_t i = 0; i < this->channels.size(); i++) {
        delete this->channels[i];
    }
}

void Server::setNonBlocking() {
    if (fcntl(this->serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("ERROR :Setting socket to non-blocking mode failed");
    }
}

void Server::setPollFds(void) {
    this->pollFDs.clear();
    struct pollfd serverPollFD;
    serverPollFD.fd = this->serverSocket;
    serverPollFD.events = POLLIN;
    this->pollFDs.push_back(serverPollFD);
    for (size_t i = 0; i < this->clientFDs.size(); i++) {
        struct pollfd clientPollFD;
        clientPollFD.fd = this->clientFDs[i];
        clientPollFD.events = POLLIN;
        this->pollFDs.push_back(clientPollFD);
    }
}

void Server::waitEvents(void) {
    int pollResult = poll(this->pollFDs.begin().base(), this->pollFDs.size(), -1);
    if (pollResult > 0) {
        for (size_t i = 0; i < this->pollFDs.size(); i++) {
            // If revents == 0 no events occurred
            if (this->pollFDs[i].revents == 0)
                continue;
            // Handle event on socket server
            if (i == 0)
                this->acceptConnection();
            // Handle event on socket clients
            else if (i > 0) {
                this->receiveData(this->pollFDs[i].fd);
            }
        }
    } else if (pollResult < 0) {
        throw std::runtime_error("ERROR :Waiting connections failed");
    }
}

void Server::acceptConnection(void) {
    int clientSocket;
    struct sockaddr_in6 clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(this->serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0 && errno != EWOULDBLOCK) {
        throw std::runtime_error("ERROR :Accepting connection failed");
    }
    this->addClientSocket(clientSocket);
    std::string ip = getClientIpAddress(&clientAddress);
    int port = ntohs(clientAddress.sin6_port);
    if (DEBUG) {
        std::cout << "New connection fd: " << clientSocket << ",ip: " << ip << ",port: " << port << std::endl;
    }
}

void Server::receiveData(int clientSocket) {
    std::string message;
    char buffer[BUFFER_SIZE];
    buffer[0] = 0;
    while (!strchr(buffer, '\n')) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0 && errno != EWOULDBLOCK) {
            throw std::runtime_error("ERROR :Receiving data from client");
        } else if (bytesRead == 0) {
            std::cout << "Client fd: " << clientSocket << " disconnected" << std::endl;
            this->delClientSocket(clientSocket);
            break;
        } else {
            message.append(std::string(buffer, bytesRead));
        }
    }
    this->handleMessage(message, clientSocket);
}

void Server::addClientSocket(int clientSocket) {
    this->clientFDs.push_back(clientSocket);
    this->setNonBlocking();
    this->setPollFds();
}

void Server::delClientSocket(int clientSocket) {
    std::vector<int>::iterator it = std::find(this->clientFDs.begin(), this->clientFDs.end(), clientSocket);
    if (it != this->clientFDs.end()) {
        this->clientFDs.erase(it);
    }
    this->setNonBlocking();
    this->setPollFds();
    close(clientSocket);
}

void Server::sendData(int clientSocket, std::string message) {
    ssize_t bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesSent < 0) {
        throw std::runtime_error("ERROR :Sending data to client");
    }
}

void Server::handleMessage(std::string message, int clientSocket) {
    trimEndOfLine(message);
    std::vector<std::string> args = splitString(message, ' ');
    if (DEBUG) {
        std::cout << "Args :";
        printVector(args);
    }
    if (args.size() == 0) {
        return;
    }
    std::string command = args[0];
    CommandHandler *cmd = NULL;
    if (command == "PASS") {
        cmd = new Pass(this);
    } else if (command == "NICK") {
        cmd = new Nick(this);
    } else if (command == "JOIN") {
        cmd = new Join(this);
    } else {
        this->sendData(clientSocket, "Error :Command not found\r\n");
        return;
    }
    cmd->execute(clientSocket, args);
};

void Server::addChannel(Channel *channel) {
    this->channels.push_back(channel);
}

void Server::delChannel(std::string channelName) {
    // Delete channel from channels vector
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i]->getNameChannel() == channelName) {
            delete this->channels[i];
            this->channels.erase(this->channels.begin() + i);
            break;
        }
    }
}

Channel *Server::getChannel(std::string channelName) {
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i]->getNameChannel() == channelName) {
            return this->channels[i];
        }
    }
    return NULL;
}

std::vector<Channel *> Server::getChannels() {
    return this->channels;
}

int Server::getClientIndex(int clientSocket) {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->getFD() == clientSocket) {
            return (int)i;
        }
    }
    return -1;
}

void Server::start(void) {
    this->serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (this->serverSocket < 0) {
        throw std::runtime_error("ERROR :Creating server socket failed");
        exit(1);
    }
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        throw std::runtime_error("ERROR :Setting socket options failed");
        close(this->serverSocket);
        exit(1);
    }
    this->setNonBlocking();
    // Initializing serverAddress
    struct sockaddr_in6 serverAddress;
    struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
    serverAddress.sin6_family = AF_INET6;
    serverAddress.sin6_addr = in6addr_any;
    serverAddress.sin6_port = htons(this->port);
    serverAddress.sin6_flowinfo = 0;
    serverAddress.sin6_scope_id = 0;
    if (bind(this->serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        throw std::runtime_error("ERROR :Binding socket to the port failed");
        close(this->serverSocket);
        exit(1);
    }
    std::cout << "IRC Server started on " << this->startTime << ". Listening for connections on port " << this->port << " ..." << std::endl;
    if (listen(this->serverSocket, MAX_CLIENTS) < 0) {
        throw std::runtime_error("ERROR :Listening for connections failed");
        close(this->serverSocket);
        exit(1);
    }
    this->setPollFds();
    while (true) {
        this->waitEvents();
    }
}
