#include "../inc/irc.hpp"

CommandHandler::CommandHandler(){};
CommandHandler::CommandHandler(Server *server) : server(server){};
CommandHandler::~CommandHandler(){};

Pass::Pass(Server *server) : CommandHandler(server){};
Pass::~Pass(){};
void Pass::execute(int clientSocket, std::vector<std::string> args) {
    Server *server = this->server;
    if (args.size() != 2) {
        server->sendData(clientSocket, "Usage : PASS <password>\r\n");
    } else {
        std::string password = args[1];
        if (password == server->getPassword()) {
            server->sendData(clientSocket, "Error :Password accepted\r\n");
        } else {
            server->sendData(clientSocket, "Error :Invalid password\r\n");
        }
    }
}

Nick::Nick(Server *server) : CommandHandler(server){};
Nick::~Nick(){};
void Nick::execute(int clientSocket, std::vector<std::string> args) {
    Server *server = this->server;
    if (args.size() != 2) {
        server->sendData(clientSocket, "Usage : NICK <nick>\r\n");
    } else {
        std::string nick = args[1];
        int clientIndex = server->getClientIndex(clientSocket);
        std::string nickname = args[1];
        bool nicknameExists = false;
        for (size_t i = 0; i < server->getRegisteredClients().size(); i++) {
            if (server->getRegisteredClients()[i]->getNickName() == nickname) {
                nicknameExists = true;
                break;
            }
        }
        if (nicknameExists) {
            server->sendData(clientSocket, "ERROR :Nickname already in use\r\n");
        } else {
            // Change nick name of client ot nickname
            server->getRegisteredClients()[clientIndex]->setNickName(nickname);
            server->sendData(clientSocket, "NICK :Nickname accepted\r\n");
        }
    }
}

Join::Join(Server *server) : CommandHandler(server){};
Join::~Join(){};
void Join::execute(int clientSocket, std::vector<std::string> args) {
    Server *server = this->server;
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(clientSocket, "Usage : JOIN <channel> <pass>\r\n");
    } else {
        std::string channelName = args[1];
        std::string channelPassword = "";
        if (args.size() == 3) {
            channelPassword = args[2];
        }
        bool channelExists = false;
        for (size_t i = 0; i < this->server->getChannels().size(); i++) {
            if (server->getChannels()[i]->getNameChannel() == channelName) {
                channelExists = true;
                break;
            }
        }
        if (channelExists) {
            // Add a new client to a channel exists
            int clientIndex = server->getClientIndex(clientSocket);
            server->getChannel(channelName)->addUser(server->getRegisteredClients()[clientIndex]);
        } else {
            // Create a new channel
            Channel *channel;
            if (channelPassword != "") {
                channel = new Channel(server, channelName, channelPassword);
            } else {
                channel = new Channel(server, channelName);
            }
            server->addChannel(channel);
            // Add a new client to a channel exists
            int clientIndex = server->getClientIndex(clientSocket);
            Client *client = server->getRegisteredClients()[clientIndex];
            server->getChannel(channelName)->addUser(client);
        }
    }
}
