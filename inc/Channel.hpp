#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"

class Client;
class Server;
class CommandHandler;

class Channel {
private:
    Server *server;
    std::string nameChannel;
    std::string topic;
    std::vector<Client *> admins;
    std::vector<Client *> users;
    bool havePassword;
    std::string password;
    bool inviteOnly;
    bool haveMaxUsers;
    int maxUsers;

public:
    Channel();
    Channel(Server *server, std::string nameChannel);
    Channel(Server *server, std::string nameChannel, std::string password);
    ~Channel();
    std::string getNameChannel(void);
    std::string getTopic(void);
    std::vector<Client *> getAdmins(void);
    std::vector<Client *> getUsers(void);
    bool getHavePassword(void);
    std::string getPassWord(void);
    bool getInviteOnly(void);
    bool getHaveMaxUsers(void);
    int getMaxUsers(void);
    Server *getServer(void);
    void setNameChannel(std::string nameChannel);
    void setTopic(std::string topic);
    void addAdmins(Client *client);
    void addAdmins(std::vector<Client *> users);
    void addUser(Client *);
    void kickUser(Client *);
    void kickUser(Client *, std::string reason);
    void invite(Client *src, Client *target);
    bool isAdmin(Client * client);
};

#endif
