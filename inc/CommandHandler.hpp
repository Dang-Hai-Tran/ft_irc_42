#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "irc.hpp"

class Server;
class Client;
class Channel;

class CommandHandler {
protected:
    Server *server;
    std::vector<std::string> args;

public:
    CommandHandler();
    CommandHandler(Server *server);
    virtual ~CommandHandler();
    virtual void execute(int clientSocket, std::vector<std::string> args) = 0;
};

// Pass class to handle PASS command
class Pass : public CommandHandler {
public:
    Pass(Server *server);
    ~Pass();
    void execute(int clientSocket, std::vector<std::string> args);
};

// Nick class to handle NICK command
class Nick : public CommandHandler {
public:
    Nick(Server *server);
    ~Nick();
    void execute(int clientSocket, std::vector<std::string> args);
};

// Join class to handle JOIN command
class Join : public CommandHandler {
public:
    Join(Server *server);
    ~Join();
    void execute(int clientSocket, std::vector<std::string> args);
};

// User class to handle USER command
// class User : public CommandHandler {
// public:
//     User(Server *server);
//     ~User();
//     void execute(int clientSocket, std::vector<std::string> args);
// };

#endif
