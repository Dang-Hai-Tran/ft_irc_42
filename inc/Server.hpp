#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_CLIENTS 32

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
    int m_nbrClients;
    int m_nbrConnections;

public:
    Client m_client[MAX_CLIENTS];
    void m_connect(void);
    void m_addClient(void);
    int m_getNbrClients(void) const;
    void m_disconnect(void);
    int m_getNbrConnections(void) const;
    Server();
    Server(int port, std::string password);
    ~Server();
    int getSocket(void);
    std::string getPassword(void);
    std::vector<Client *> &getRegisteredClients(void);
    std::vector<int> &getClientFDs(void);
    void waitEvents(void);
    void acceptConnection(void);
    void receiveData(int clientSocket);
    void sendData(int clientSocket, std::string message);
    void addClientSocket(int clientSocket);
    void delClientSocket(int clientSocket);
    void addChannel(Channel *channel);
    void delChannel(std::string channelName);
    Channel *getChannel(std::string channelName);
    std::vector<Channel *> &getChannels(void);
    int getClientIndex(int clientSocket);
    Client *getClient(int clientSocket);
    void start(void);
    std::vector<Client *> getOnServerClients(void);
};

#endif
