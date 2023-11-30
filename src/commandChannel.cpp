#include "../inc/irc.hpp"

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void commandJoin(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(client->m_getSocket(), "Usage: JOIN <channel>{,<channel>} [<key>{,<key>}]\r\n");
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    std::vector<std::string> listPassword = splitString(args[2], ',');
    if (listChannelName.size() != listPassword.size() && listPassword.size() != 0) {
        server->sendData(client->m_getSocket(), "Usage: JOIN <channel>,<channel>... <password>,<password>...\r\n");
        return;
    }
    for (size_t i = 0; i < listChannelName.size(); i++) {
        Channel *channel = server->getChannel(listChannelName[i]);
        if (channel == NULL) {
            if (args.size() == 2)
                channel = new Channel(server, listChannelName[i]);
            else
                channel = new Channel(server, listChannelName[i], listPassword[i]);
            server->addChannel(channel);
        }
        // Check if user is already in channel
        if (channel->isInChannel(client)) {
            server->sendData(client->m_getSocket(), "You are already in channel " + channel->getNameChannel() + "\r\n");
            continue;
        }
        // Check if channel is invite only, user is invited
        if (channel->getInviteOnly() && !channel->isInvited(client)) {
            server->sendData(client->m_getSocket(), "You are not invited to channel " + channel->getNameChannel() + "\r\n");
            continue;
        }
        // Check if password is correct
        if (channel->getHavePassword() && channel->getPassword() != listPassword[i]) {
            server->sendData(client->m_getSocket(), "Password of " + channel->getNameChannel() + " incorrect\r\n");
            continue;
        }
        // Check if channel have full users
        if (channel->getHaveMaxUsers() && (int)channel->getNumberUsers() >= channel->getMaxUsers()) {
            server->sendData(client->m_getSocket(), "Channel " + channel->getNameChannel() + " is full\r\n");
            continue;
        }
        channel->addUser(client);
        client->addChannel(channel);
        if (client->m_getStatusC() == false)
            client->m_setStatusC(true);
        if (channel->getNumberUsers() == 1)
            channel->addAdmins(client);
        std::cout << client->m_getNickName() << " joining channel " << channel->getNameChannel() << std::endl;
        // Send join message to all users in channel
        channel->sendMessageToAll(client->m_getNickName() + " has joined the channel " + channel->getNameChannel());
        // Send topic to user joining channel
        if (channel->getTopic() != "")
            server->sendData(client->m_getSocket(), "Topic of channel " + channel->getNameChannel() + ": " + channel->getTopic() + "\r\n");
    }
}

// PART <channel>{,<channel>} [<reason>]
void commandPart(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(client->m_getSocket(), "Usage: PART <channel>{,<channel>} [<reason>]\r\n");
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    std::string reason = "";
    if (args.size() == 2)
        reason = args[2];
    for (size_t i = 0; i < listChannelName.size(); i++) {
        Channel *channel = server->getChannel(listChannelName[i]);
        if (channel == NULL) {
            server->sendData(client->m_getSocket(), "No such channel\r\n");
            continue;
        }
        channel->delUser(client);
        client->delChannel(channel);
        if (channel->getNumberUsers() == 0) {
            server->delChannel(channel->getNameChannel());
            delete channel;
        }
        std::cout << client->m_getNickName() << " leaving channel " << channel->getNameChannel() << std::endl;
        // Send leaving message to all users in channel
        if (reason == "")
            channel->sendMessageToAll(client->m_getNickName() + " has left the channel " + channel->getNameChannel());
        else
            channel->sendMessageToAll(client->m_getNickName() + " has left the channel " + channel->getNameChannel() + " :" + reason);
    }
    if (client->getChannelsUserIn().size() == 0)
        client->m_setStatusC(false);
}

// TOPIC <channel> [<topic>]
void commandTopic(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(client->m_getSocket(), "Usage: TOPIC <channel> [<topic>]\r\n");
        return;
    }
    Channel *channel = server->getChannel(args[1]);
    if (channel == NULL) {
        server->sendData(client->m_getSocket(), "No such channel " + args[1] + "\r\n");
        return;
    }
    if (channel->getTopicRestriction() == true) {
        // Check if user is admin of channel
        if (!channel->isAdmin(client)) {
            server->sendData(client->m_getSocket(), "You are not admin of channel " + args[1] + "\r\n");
            return;
        }
    }
    // If args.size() == 2, send to topic to client
    // If args.size() == 3, set topic of channel to args[2] or clear topic if args[2] = ""
    if (args.size() == 2) {
        server->sendData(client->m_getSocket(), "Topic of channel " + channel->getNameChannel() + " : " + channel->getTopic() + "\r\n");
    }
    if (args.size() == 3) {
        channel->setTopic(args[2]);
        if (args[2].empty())
            channel->sendMessageToAll("Topic of channel " + channel->getNameChannel() + " cleared");
        else
            channel->sendMessageToAll("Topic for " + channel->getNameChannel() + " set to : " + channel->getTopic());
    }
    std::cout << client->m_getNickName() << " set topic of channel " << channel->getNameChannel() << " to " << channel->getTopic() << std::endl;
}

// NAMES <channel>{,<channel>} : See all users in channel
void commandNames(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() != 2) {
        server->sendData(client->m_getSocket(), "Usage: NAMES <channel>{,<channel>}\r\n");
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    for (size_t i = 0; i < listChannelName.size(); i++) {
        Channel *channel = server->getChannel(listChannelName[i]);
        if (channel == NULL) {
            server->sendData(client->m_getSocket(), "No such channel : " + listChannelName[i] + "\r\n");
            continue;
        }
        std::string listUsers = "";
        for (size_t j = 0; j < channel->getNumberUsers(); j++) {
            listUsers += channel->getUsers()[j]->m_getNickName() + " ";
        }
        server->sendData(client->m_getSocket(), "List users in channel " + channel->getNameChannel() + " : " + listUsers + "\r\n");
    }
}

// INVITE <nickname> <channel> : Invite user to channel
void commandInvite(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() != 3) {
        server->sendData(client->m_getSocket(), "Usage: INVITE <nickname> <channel>\r\n");
        return;
    }
    // Find target user on server by nickname
    Client *target = NULL;
    for (size_t i = 0; i < server->getOnServerClients().size(); i++) {
        if (server->getOnServerClients()[i]->m_getNickName() == args[1]) {
            target = server->getOnServerClients()[i];
            break;
        }
    }
    if (target == NULL) {
        server->sendData(client->m_getSocket(), "No such user : " + args[1] + "\r\n");
        return;
    }
    Channel *channel = server->getChannel(args[2]);
    if (channel == NULL) {
        server->sendData(client->m_getSocket(), "No such channel : " + args[2] + "\r\n");
        return;
    }
    channel->invite(client, target);
}

// KICK <channel> <nickname>{,<nickname>} [<reason>]
void commandKick(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() < 3 || args.size() > 4) {
        server->sendData(client->m_getSocket(), "Usage: KICK <channel> <nickname>{,<nickname>} [<reason>]\r\n");
        return;
    }
    Channel *channel = server->getChannel(args[1]);
    if (channel == NULL) {
        server->sendData(client->m_getSocket(), "No such channel : " + args[1] + "\r\n");
        return;
    }
    // Check if user is admin of channel
    if (!channel->isAdmin(client)) {
        server->sendData(client->m_getSocket(), "You are not admin of this channel\r\n");
        return;
    }
    std::vector<std::string> listNickName = splitString(args[2], ',');
    std::string reason = "";
    if (args.size() == 4)
        reason = args[3];
    for (size_t i = 0; i < listNickName.size(); i++) {
        Client *target = channel->getUserByNickName(listNickName[i]);
        if (target == NULL) {
            server->sendData(client->m_getSocket(), "No such user : " + listNickName[i] + "\r\n");
            continue;
        }
        // Check if target is in channel
        if (!channel->isInChannel(target)) {
            server->sendData(client->m_getSocket(), "User " + listNickName[i] + " is not in channel " + channel->getNameChannel() + "\r\n");
            continue;
        }
        channel->kickUser(target, reason);
    }
}

// MODE <channel> <mode> <arg>... : Change mode of channel
// Accept only +-i, +-t, +-k, +-o, +-l
void commandMode(Server *server, Client *client, std::string message) {
    std::vector<std::string> args = splitString(message, ' ');
    if (args.size() < 3 || args.size() > 4) {
        server->sendData(client->m_getSocket(), "MODE <channel> <mode> <arg>\r\n");
        return;
    }
    Channel *channel = server->getChannel(args[1]);
    if (channel == NULL) {
        server->sendData(client->m_getSocket(), "No such channel : " + args[1] + "\r\n");
        return;
    }
    // Check if user is admin of channel
    if (!channel->isAdmin(client)) {
        server->sendData(client->m_getSocket(), "You are not admin of this channel\r\n");
        return;
    }
    std::string mode = args[2];
    std::string arg = "";
    if (args.size() == 4)
        arg = args[3];
    // Check if mode is valid
    if (mode != "+i" && mode != "-i" && mode != "+t" && mode != "-t" && mode != "+k" && mode != "-k" && mode != "+o" && mode != "-o" && mode != "+l" && mode != "-l") {
        server->sendData(client->m_getSocket(), "Invalid mode " + mode + "\r\n");
        return;
    }
    if (mode[0] == '+') {
        if (mode == "+i") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +i\r\n");
                return;
            }
            channel->setInviteOnly(true);
        }
        if (mode == "+t") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +t\r\n");
                return;
            }
            channel->setTopicRestriction(true);
        }
        if (mode == "+k") {
            if (arg == "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +k <password>\r\n");
                return;
            }
            channel->setHavePassword(true);
            channel->setPassword(arg);
        }
        if (mode == "+o") {
            if (arg == "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +o <nickname>\r\n");
                return;
            }
            Client *client = channel->getUserByNickName(arg);
            if (client == NULL) {
                server->sendData(client->m_getSocket(), "No such user " + arg + "\r\n");
                return;
            }
            channel->addAdmins(client);
        }
        if (mode == "+l") {
            if (arg == "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +l <limit>\r\n");
                return;
            }
            if (strContainOnlyDigits(arg) == false) {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> +l <limit> with <limit> is number");
                return;
            }
            channel->setHaveMaxUsers(true);
            channel->setMaxUsers(std::atoi(arg.c_str()));
        }
    }
    if (mode[0] == '-') {
        if (mode == "-i") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> -i\r\n");
                return;
            }
            channel->setInviteOnly(false);
        }
        if (mode == "-t") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> -t\r\n");
                return;
            }
            channel->setTopicRestriction(false);
        }
        if (mode == "-k") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> -k\r\n");
                return;
            }
            channel->setHavePassword(false);
            channel->setPassword("");
        }
        if (mode == "-o") {
            if (arg == "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> -o <nickname>\r\n");
                return;
            }
            Client *client = channel->getUserByNickName(arg);
            if (client == NULL) {
                server->sendData(client->m_getSocket(), "No such user " + arg + "\r\n");
                return;
            }
            if (channel->isAdmin(client) == false) {
                server->sendData(client->m_getSocket(), arg + "is not admin\r\n");
                return;
            }
            channel->delAdmin(client);
        }
        if (mode == "-l") {
            if (arg != "") {
                server->sendData(client->m_getSocket(), "Usage: MODE <channel> -l\r\n");
                return;
            }
            channel->setHaveMaxUsers(false);
            channel->setMaxUsers(std::numeric_limits<int>::max());
        }
    }
}
