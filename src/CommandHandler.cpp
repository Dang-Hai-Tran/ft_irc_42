#include "../inc/irc.hpp"

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
            server->sendData(clientSocket, "Password accepted\r\n");
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
            // Change nick name of client to nickname
            Client *client = server->getClient(clientSocket);
            client->setNickName(nickname);
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
        Client *client = server->getClient(clientSocket);
        if (channelExists) {
            // Add a new client to a channel exists
            server->getChannel(channelName)->addUser(client);
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
            server->getChannel(channelName)->addUser(client);
        }
    }
}

Part::Part(Server *server) : CommandHandler(server){};
Part::~Part(){};
void Part::execute(int clientSocket, std::vector<std::string> args) {
    Server *server = this->server;
    Client *client = server->getClient(clientSocket);
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(clientSocket, "Usage : PART <channel> <message>\r\n");
    } else {
        std::string channelName = args[1];
        std::string message = "";
        if (args.size() == 3) {
            message = args[2];
        }
        Channel *channel = server->getChannel(channelName);
        channel->kickUser(client);
        std::vector<Client *> clients = channel->getUsers();
        for (size_t i = 0; i < clients.size(); i++) {
            std::string leaveMessage = clients[i]->getNickName() + "leave " + channelName + " :" + message + "\r\n";
            server->sendData(clients[i]->getFD(), leaveMessage);
        }
    }
}

// Implement Topic Class
Topic::Topic(Server *server) : CommandHandler(server){};
Topic::~Topic(){};
void Topic::execute(int clientSocket, std::vector<std::string> args) {
    Server *server = this->server;
    Client *client = server->getClient(clientSocket);
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(clientSocket, "Usage : TOPIC <channel> <topic>\r\n");
    } else {
        std::string channelName = args[1];
        std::string topic = "";
        if (args.size() == 3) {
            topic = args[2];
        }
        Channel *channel = server->getChannel(channelName);
        if (channel->isAdmin(client) == false) {
            server->sendData(clientSocket, "ERROR :You are not admin\r\n");
        } else {
            channel->setTopic(topic);
            std::string topicMessage = "You change topic of channel: " + channelName + " to " + topic + "\r\n";
            server->sendData(client->getFD(), topicMessage);
        }
    }
}
