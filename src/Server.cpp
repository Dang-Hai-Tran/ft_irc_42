#include "../inc/irc.hpp"

Server::Server(){};
Server::Server(int port, std::string password) : port(port), password(password) {
    this->startTime = getCurrentTime();
    this->serverName = "irc.42.fr";
};

std::vector<Client *> &Server::getRegisteredClients(void) {
    return this->registeredClients;
};

std::vector<int> &Server::getClientFDs(void) {
    return this->clientFDs;
}

int Server::getSocket(void) {
    return this->serverSocket;
}

std::string Server::getPassword(void) {
    return this->password;
}

Server::~Server() {
    for (size_t i = 0; i < this->m_listConnection.size(); i++) {
        if (this->m_listConnection[i]->m_getID() == 0) {
            delete this->m_listConnection[i];
        }
    }
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        delete this->registeredClients[i];
    }
    this->m_listConnection.clear();
    this->registeredClients.clear();
    for (size_t i = 0; i < this->channels.size(); i++) {
        delete this->channels[i];
    }
    this->channels.clear();
}

void Server::setNonBlocking() {
    if (fcntl(this->serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("ERROR :Setting socket to non-blocking mode failed");
    }
}

void Server::setPollFds(void) {
    if (this->pollFDs.size() > 0)
        this->pollFDs.clear();
    struct pollfd serverPollFD;
    serverPollFD.fd = this->serverSocket;
    serverPollFD.events = POLLIN;
    serverPollFD.revents = 0;
    this->pollFDs.push_back(serverPollFD);
    for (size_t i = 0; i < this->clientFDs.size(); i++) {
        struct pollfd clientPollFD;
        clientPollFD.fd = this->clientFDs[i];
        clientPollFD.events = POLLIN;
        clientPollFD.revents = 0;
        this->pollFDs.push_back(clientPollFD);
    }
}

void Server::waitEvents(void) {
    int pollResult = poll(this->pollFDs.begin().base(), this->pollFDs.size(), -1);
    if (pollResult > 0) {
        for (size_t i = 0; i < this->pollFDs.size(); i++) {
            // If revents == 0 no events occurred
            if (this->pollFDs[i].revents & POLLIN) {
                // Handle event on socket server
                if (i == 0)
                    this->acceptConnection();
                // Handle event on socket clients
                else if (i > 0) {
                    this->receiveData(this->pollFDs[i].fd);
                }
            }
            if (this->pollFDs[i].revents & POLLHUP) {
                std::cout << "Connection lost" << std::endl;
            }
        }
    } else if (pollResult < 0) {
        throw std::runtime_error("ERROR :Waiting connections failed");
    }
}

// xuluu
void ft_add_connection(Server &server, int socket) {
    Client *client = new Client();
    client->m_setSocket(socket);
    server.m_getListConnection().push_back(client);
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
    std::string ip = getIpAddressFromSockaddr(&clientAddress);
    int port = ntohs(clientAddress.sin6_port);
    std::cout << "New connection from client fd " << clientSocket << ",ip: " << ip << ",port: " << port << std::endl;
    ft_add_connection(*this, clientSocket);
}

void ft_input(Server &server, int socket, std::string &input) {
    size_t i(0);
    std::vector<Client *> clients = server.m_getListConnection();

    while (i < clients.size()) {
        int sk = clients[i]->m_getSocket();
        if (sk == socket) {
            clients[i]->m_setInput(input);
            get_input(server, clients[i]);
            break;
        }
        i++;
    }
}

void Server::receiveData(int clientSocket) {
    std::string message;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
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
    std::cout << std::left << std::setw(40) << "[Client] Message received from client " << clientSocket << " << " << message;
    if (message.size() > 0)
        ft_input(*this, clientSocket, message);
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
    // xuluu
    size_t i(0);
    while (i < m_getListConnection().size()) {
        if (m_getListConnection()[i]->m_getSocket() == clientSocket) {
            Client *client = m_getListConnection()[i];
            reset_data(client);
            std::vector<Channel *> listChannel = client->getChannelsUserIn();
            for (size_t j = 0; j < listChannel.size(); j++) {
                listChannel[j]->delUser(client);
                if (listChannel[j]->isAdmin(client)) {
                    listChannel[j]->delAdmin(client);
                }
            }
            listChannel.clear();
            m_getListConnection().erase(m_getListConnection().begin() + i);
            delete client;
            break;
        }
        i++;
    }
    close(clientSocket);
}

void Server::sendData(int clientSocket, std::string message) {
    ssize_t bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesSent < 0) {
        throw std::runtime_error("ERROR :Sending data to client");
    }
    std::cout << std::left << std::setw(40) << "[Server] Message sent to client " << clientSocket << " >> " << message;
}

void Server::addChannel(Channel *channel) {
    this->channels.push_back(channel);
}

void Server::delChannel(std::string channelName) {
    // Delete channel from channels vector
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i]->getNameChannel() == channelName) {
            delete this->channels[i];
            this->channels.erase(this->channels.begin() + i);
            return;
        }
    }
}

void Server::delChannel(Channel *channel) {
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i] == channel) {
            delete channel;
            this->channels.erase(this->channels.begin() + i);
            return;
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

std::vector<Channel *> &Server::getChannels() {
    return this->channels;
}

int Server::getClientIndex(int clientSocket) {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getSocket() == clientSocket) {
            return (int)i;
        }
    }
    return -1;
}

Client *Server::getClient(int clientSocket) {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getSocket() == clientSocket) {
            return this->registeredClients[i];
        }
    }
    return NULL;
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

std::vector<Client *> &Server::m_getListConnection(void) {
    return (this->m_listConnection);
}

std::vector<Client *> Server::getOnServerClients(void) {
    std::vector<Client *> onServerClients;
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getStatusS() == true) {
            onServerClients.push_back(this->registeredClients[i]);
        }
    }
    return onServerClients;
}

void Server::sendData(Client *client, std::string message) {
    this->sendData(client->m_getSocket(), message);
}

std::string Server::getServerName(void) {
    return this->serverName;
}

Client *Server::getClientByNickName(std::string nick) {
    Client *target = NULL;
    for (size_t i = 0; i < this->getOnServerClients().size(); i++) {
        if (this->getOnServerClients()[i]->m_getNickName() == nick) {
            target = this->getOnServerClients()[i];
            break;
        }
    }
    return target;
}
