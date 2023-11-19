#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "Server.hpp"

class Server;

class Client {
private:
    int fd;
    std::string ip;
    int port;
    bool correctPassword;
    std::string nickName;
    std::string userName;
    std::string realName;
    // std::vector<Channel *> userChannels;
    std::string partialRecv;
    Server *server;

public:
    Client(Server *server, int fd, std::string ip, int port);
    ~Client();
    int getFD(void);
    std::string getIP(void);
    int getPort(void);
    std::string getNickName(void);
    std::string getUserName(void);
    std::string getRealName(void);
    std::string getPartialRecv(void);
    // std::vector<Channel *> getUserChannels(void);
    void setFD(int fd);
    void setIP(std::string ip);
    void setPort(int port);
    void setNickName(std::string nickName);
    void setUserName(std::string userName);
    void setRealName(std::string realName);
    void setPartialRecv(std::string partialRecv);
    // void joinChannel(Channel *channel);
    // void leaveChannel(Channel *channel, bool kicked, std::string reason);
};

#endif
