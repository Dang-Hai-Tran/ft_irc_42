#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"

class Server;
class Channel;
class CommandHandler;

class Client {
private:
    Server *server;
    int fd;
    std::string nickName;
    std::string userName;
    std::string realName;

public:
    Client(Server *server, int fd);
    ~Client();
    Server *getServer(void);
    int getFD(void);
    std::string getNickName(void);
    std::string getUserName(void);
    std::string getRealName(void);
    void setFD(int fd);
    void setNickName(std::string nickName);
    void setUserName(std::string userName);
    void setRealName(std::string realName);
};

#endif
