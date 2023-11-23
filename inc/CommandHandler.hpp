#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "irc.hpp"

class Server;
class Client;
class Channel;

class CommandHandler {
protected:
    Server *server;
    std::string command;
    std::vector<std::string> args;
    std::string message;

public:
    CommandHandler();
    CommandHandler(Server *server);
    virtual ~CommandHandler();
    virtual void execute(int clientSocket, std::vector<std::string> args) = 0;
};

class Pass : public CommandHandler {
public:
    Pass(Server *Server);
    ~Pass();
    void execute(int clientSocket, std::vector<std::string> args); 
};

#endif
