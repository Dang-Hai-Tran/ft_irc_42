#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "irc.hpp"

class Client;
class Server;

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
    std::string mode;

public:
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
    void delAdmin(Client *client);
    void addUser(Client *);
    void delUser(Client *);
    bool isAdmin(Client *client);
    size_t getNumberUsers(void);
    size_t getNumberAdmins(void);
    bool isInChannel(Client *client);
    void sendMessageToAll(std::string message);
    bool isInvited(Client *client);
    Client *getUserByNickName(std::string nickName);
    std::string getListOfMembers(void);
    void addToInvited(Client *client);
    void delFromInvited(Client *client);
    std::string &getMode(void);
    void addMode(std::string mode);
    void delMode(std::string mode);
};

#endif
