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
    CommandHandler(Server *server);
    virtual ~CommandHandler();
    virtual void execute(int clientSocket, std::vector<std::string> args) = 0;
};

// Pass class to handle PASS command
// PASS <password>
class Pass : public CommandHandler {
public:
    Pass(Server *server);
    ~Pass();
    void execute(int clientSocket, std::vector<std::string> args);
};

// Nick class to handle NICK command
// NICK <nickname>
class Nick : public CommandHandler {
public:
    Nick(Server *server);
    ~Nick();
    void execute(int clientSocket, std::vector<std::string> args);
};

// Join class to handle JOIN command
// JOIN <channel> <password>
class Join : public CommandHandler {
public:
    Join(Server *server);
    ~Join();
    void execute(int clientSocket, std::vector<std::string> args);
};

// Part class to handle PART command
// PART <channel> <message>
class Part : public CommandHandler {
public:
    Part(Server *server);
    ~Part();
    void execute(int clientSocket, std::vector<std::string> args); 
};

#endif
