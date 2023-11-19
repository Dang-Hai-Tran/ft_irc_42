#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"

#define DEFAULT_SERVER_NAME "IRC Server"
#define DEFAULT_PORT 8888
#define MAX_CLIENTS 32
#define BUFFER_SIZE 8192
#ifndef DEBUG
#define DEBUG 0
#endif

class Client;

class Server {
private:
    int port;
    std::string password;
    std::string serverName;
    std::string startTime;
    int serverSocket;
    std::vector<Client *> clients;
    // std::vector<Channel *> channels;
    struct pollfd *clientFds;
    int setNonBlocking(int serverSocket);
    void setClientFds(void);

public:
    Server(int port, std::string password);
    ~Server();
    void waitEvents(void);
    void acceptConnection(void);
    void receiveData(Client *);
    void addClient(int clientSocket, std::string clientIpAddress, int clientPort);
    void delClient(int clientSocket);
    void handleCommand(std::string command, Client *client);
    int listening(void);
};

#endif
