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
    std::vector<Client *>   m_listConnection;
    std::string serverName;


public:
    std::vector<Client *>   &m_getListConnection(void);
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
    void sendData(Client *client, std::string message);
    std::string getServerName(void);
    void delChannel(Channel *channel);
    Client *getClientByNickName(std::string nick);
};

#endif
