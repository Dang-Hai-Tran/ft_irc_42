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
    std::vector<Client *> invited;
    bool havePassword;
    std::string password;
    bool inviteOnly;
    bool haveMaxUsers;
    int maxUsers;
    bool topicRestrictions;

public:
    Channel(Server *server, std::string nameChannel);
    Channel(Server *server, std::string nameChannel, std::string password);
    ~Channel();
    std::string getNameChannel(void);
    std::string getTopic(void);
    std::vector<Client *> &getAdmins(void);
    std::vector<Client *> &getUsers(void);
    std::vector<Client *> &getInvited(void);
    bool getHavePassword(void);
    std::string getPassword(void);
    bool getInviteOnly(void);
    bool getHaveMaxUsers(void);
    int getMaxUsers(void);
    Server *getServer(void);
    bool getTopicRestriction();
    void setNameChannel(std::string nameChannel);
    void setTopic(std::string topic);
    void setHavePassword(bool havePassword);
    void setPassword(std::string password);
    void setInviteOnly(bool inviteOnly);
    void setHaveMaxUsers(bool haveMaxUsers);
    void setMaxUsers(int maxUsers);
    void setTopicRestriction(bool topicRestriction);
    void addAdmins(Client *client);
    void addAdmins(std::vector<Client *> users);
    void delAdmin(Client *client);
    void addUser(Client *);
    void delUser(Client *);
    void kickUser(Client *, std::string reason);
    void invite(Client *src, Client *target);
    bool isAdmin(Client *client);
    size_t getNumberUsers(void);
    size_t getNumberAdmins(void);
    bool isInChannel(Client *client);
    void sendMessageToAll(std::string message);
    bool isInvited(Client *client);
    Client *getUserByNickName(std::string nickName);
};

#endif
