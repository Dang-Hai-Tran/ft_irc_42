#include "../inc/irc.hpp"

/**
 * @brief Default constructor for the Server class.
 */
Server::Server(){};

/**
 * @brief Parameterized constructor for the Server class.
 * @param port The port number for the server.
 * @param password The password for the server.
 */
Server::Server(int port, std::string password) : port(port), password(password) {
    this->startTime = getCurrentTime();
    this->serverName = "irc.42.fr";
};

/**
 * @brief Get the vector of registered clients.
 * @return A reference to the vector of registered clients.
 */
std::vector<Client *> &Server::getRegisteredClients(void) {
    return this->registeredClients;
};

/**
 * @brief Get the vector of client file descriptors.
 * @return A reference to the vector of client file descriptors.
 */
std::vector<int> &Server::getClientFDs(void) {
    return this->clientFDs;
}

/**
 * @brief Get the server socket.
 * @return The server socket.
 */
int Server::getSocket(void) {
    return this->serverSocket;
}

/**
 * @brief Get the server password.
 * @return The server password.
 */
std::string Server::getPassword(void) {
    return this->password;
}

/**
 * @brief Destructor for the Server class.
 */
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

/**
 * @brief Set the server socket to non-blocking mode.
 * @throws std::runtime_error if setting socket to non-blocking mode fails.
 */
void Server::setNonBlocking() {
    if (fcntl(this->serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        throw std::runtime_error("ERROR :Setting socket to non-blocking mode failed");
    }
}

/**
 * @brief Set the poll file descriptors for the server.
 */
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

/**
 * @brief Wait for events on the server.
 * @throws std::runtime_error if waiting for connections fails.
 */
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

/**
 * @brief Add a connection to the server.
 * @param server The server object.
 * @param socket The socket for the connection.
 */
void ft_add_connection(Server &server, int socket) {
    Client *client = new Client();
    std::cout << "1 --> " << client << std::endl;
    client->m_setSocket(socket);
    server.m_getListConnection().push_back(client);
}

/**
 * @brief Accept a connection on the server.
 */
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

/**
 * @brief Handle input from a client.
 * @param server The server object.
 * @param socket The socket for the client.
 * @param input The input from the client.
 */
void ft_input(Server &server, int socket, std::string &input) {
    size_t i(0);
    std::vector<Client *> clients = server.m_getListConnection();

    while (i < clients.size()) {
        int sk = clients[i]->m_getSocket();
        if (sk == socket) {
            Client *tmp = clients[i];
            clients[i]->m_setInput(input);
            get_input(server, clients[i]);

            // new --> old
            if (tmp != clients[i]) {
                delete tmp;
                server.m_getListConnection()[i] = clients[i];
            }
            std::cout << "3 --> " << clients[i] << std::endl;
            break;
        }
        i++;
    }
}

/**
 * @brief Receive data from a client.
 * @param clientSocket The socket for the client.
 */
void Server::receiveData(int clientSocket) {
    static std::string message;
    char buffer[BUFFER_SIZE + 1];
    memset(buffer, 0, sizeof(buffer));
    while (!strchr(buffer, '\n')) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
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
    size_t pos = message.find("\n");
    std::string line = message.substr(0, pos);
    std::cout << std::left << std::setw(40) << "[Client] Message received from client " << clientSocket << " << " << line;
    if (line.size() > 0)
        ft_input(*this, clientSocket, message);
    message.erase(0, pos + 1);
}

/**
 * @brief Add a client socket to the server.
 * @param clientSocket The client socket to add.
 */
void Server::addClientSocket(int clientSocket) {
    this->clientFDs.push_back(clientSocket);
    this->setNonBlocking();
    this->setPollFds();
}

/**
 * @brief Delete a client socket from the server.
 * @param clientSocket The client socket to delete.
 */
void Server::delClientSocket(int clientSocket) {
    std::vector<int>::iterator it = std::find(this->clientFDs.begin(), this->clientFDs.end(), clientSocket);
    if (it != this->clientFDs.end()) {
        this->clientFDs.erase(it);
    }
    this->setNonBlocking();
    this->setPollFds();

    // xuluu
    size_t i(0);
    while (i < this->m_getListConnection().size()) {
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
            if (client->m_getID() == 0)
                delete client;
            break;
        }
        i++;
    }
    close(clientSocket);
}

/**
 * @brief Send data to a client.
 * @param clientSocket The socket for the client.
 * @param message The message to send.
 */
void Server::sendData(int clientSocket, std::string message) {
    ssize_t bytesSent = send(clientSocket, message.c_str(), message.length(), 0);
    if (bytesSent < 0) {
        throw std::runtime_error("ERROR :Sending data to client");
    }
    std::cout << std::left << std::setw(40) << "[Server] Message sent to client " << clientSocket << " >> " << message;
}

/**
 * @brief Add a channel to the server.
 * @param channel The channel to add.
 */
void Server::addChannel(Channel *channel) {
    this->channels.push_back(channel);
}

/**
 * @brief Delete a channel from the server by channel name.
 * @param channelName The name of the channel to delete.
 */
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

/**
 * @brief Delete a channel from the server by channel object.
 * @param channel The channel to delete.
 */
void Server::delChannel(Channel *channel) {
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i] == channel) {
            delete channel;
            this->channels.erase(this->channels.begin() + i);
            return;
        }
    }
}

/**
 * @brief Get a channel from the server by channel name.
 * @param channelName The name of the channel to get.
 * @return The channel object, or NULL if not found.
 */
Channel *Server::getChannel(std::string channelName) {
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i]->getNameChannel() == channelName) {
            return this->channels[i];
        }
    }
    return NULL;
}

/**
 * @brief Get the vector of channels on the server.
 * @return A reference to the vector of channels.
 */
std::vector<Channel *> &Server::getChannels() {
    return this->channels;
}

/**
 * @brief Get the index of a client in the registered clients vector.
 * @param clientSocket The socket of the client.
 * @return The index of the client, or -1 if not found.
 */
int Server::getClientIndex(int clientSocket) {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getSocket() == clientSocket) {
            return (int)i;
        }
    }
    return -1;
}

/**
 * @brief Get a client object by client socket.
 * @param clientSocket The socket of the client.
 * @return The client object, or NULL if not found.
 */
Client *Server::getClient(int clientSocket) {
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getSocket() == clientSocket) {
            return this->registeredClients[i];
        }
    }
    return NULL;
}

/**
 * @brief Start the server.
 */
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

/**
 * @brief Get the list of connections on the server.
 * @return A reference to the vector of connections.
 */
std::vector<Client *> &Server::m_getListConnection(void) {
    return (this->m_listConnection);
}

/**
 * @brief Get the list of clients currently on the server.
 * @return A vector of client objects.
 */
std::vector<Client *> Server::getOnServerClients(void) {
    std::vector<Client *> onServerClients;
    for (size_t i = 0; i < this->registeredClients.size(); i++) {
        if (this->registeredClients[i]->m_getStatusS() == true) {
            onServerClients.push_back(this->registeredClients[i]);
        }
    }
    return onServerClients;
}

/**
 * @brief Send data to a client.
 * @param client The client object.
 * @param message The message to send.
 */
void Server::sendData(Client *client, std::string message) {
    this->sendData(client->m_getSocket(), message);
}

/**
 * @brief Get the server name.
 * @return The server name.
 */
std::string Server::getServerName(void) {
    return this->serverName;
}

/**
 * @brief Get a client object by nickname.
 * @param nick The nickname of the client.
 * @return The client object, or NULL if not found.
 */
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
