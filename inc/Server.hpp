#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"

class Client;
class Channel;

class Server {
private:
    int port;
    std::string password;
    std::string startTime;
    int serverSocket;
    std::vector<int> clientFDs;
    std::vector<struct pollfd> pollFDs;
    std::vector<Client *> registeredClients;
    std::vector<Channel *> channels;
    std::vector<Client *> admins;
    void setNonBlocking();
    void setPollFds(void);

public:
    Server();
    Server(int port, std::string password);
    ~Server();
    int getServerSocket(void);
    std::vector<Client *> getRegisteredClients(void);
    std::vector<int> getClientFDs(void);
    void waitEvents(void);
    void acceptConnection(void);
    void receiveData(int clientSocket);
    void sendData(int clientSocket, std::string message);
    void addClientSocket(int clientSocket);
    void delClientSocket(int clientSocket);
    void handleMessage(std::string message, int clientSocket);
    void addChannel(Channel *channel);
    void delChannel(std::string channelName);
    Channel *getChannel(std::string channelName);
    int getClientIndex(int clientSocket);
    void start(void);
};

#endif
