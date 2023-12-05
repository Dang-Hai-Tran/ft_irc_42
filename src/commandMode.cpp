#include "../inc/irc.hpp"

static void setModeToChannel(Server *server, Client *client, Channel *channel, char modeOperator, char modeCommand, std::string param);

// MODE <channel> <mode> <arg>... : Change mode of channel
// Accept only +-i, +-t, +-k, +-o, +-l
void commandMode(Server *server, Client *client, std::string input) {
    std::string nick = client->m_getNickName();
    std::string username = client->m_getUserName();
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() < 2) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "MODE"));
        return;
    }
    std::string channelName = args[1];
    Channel *channel = server->getChannel(channelName);
    if (channel == NULL) {
        server->sendData(client, ERR_NOSUCHCHANNEL(nick, channelName));
        return;
    }
    std::string channelMode = channel->getMode();
    std::string channelKey = channel->getPassword();
    // If <mode> is not given
    if (args.size() == 2) {
        if (channel->isInChannel(client) && channel->getHavePassword())
            server->sendData(client, RPL_CHANNELMODEISWITHKEY(nick, channelName, channelMode, channelKey));
        else
            server->sendData(client, RPL_CHANNELMODEIS(nick, channelName, channelMode));
        return;
    }
    // Check if user is admin of channel
    if (!channel->isAdmin(client)) {
        server->sendData(client->m_getSocket(), ERR_CHANOPRIVSNEEDED(nick, channelName));
        return;
    }
    std::string mode = args.size() >= 3 ? args[2] : "";
    std::string param = "";
    if (args.size() >= 4) {
        param = args[3];
    }
    std::vector<std::string> vectorMode = parseModeString(mode);
    if (DEBUG) {
        std::cout << "Mode: " << mode << std::endl;
        std::cout << "VectorMode: " << vectorMode << std::endl;
        std::cout << "Param: " << param << std::endl;
    }
    if (vectorMode.empty()) {
        server->sendData(client, ERR_UMODEUNKNOWNFLAG(nick));
        return;
    }
    for (size_t i = 0; i < vectorMode.size(); i++) {
        std::string setMode = vectorMode[i];
        char modeOperator = setMode[0];
        std::string modeCommand = setMode.substr(1);
        if (modeCommand.empty()) {
            server->sendData(client, ERR_UMODEUNKNOWNFLAG(nick));
            return;
        }
        for (size_t i = 0; i < modeCommand.size(); i++) {
            setModeToChannel(server, client, channel, modeOperator, modeCommand[i], param);
        }
    }
}

void setModeToChannel(Server *server, Client *client, Channel *channel, char modeOperator, char modeCommand, std::string param) {
    std::string channelName = channel->getNameChannel();
    std::string srcNick = client->m_getNickName();
    if (modeCommand == 'i') {
        if (!param.empty()) {
            server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "i", param, "Param is not empty"));
            return;
        }
        if (modeOperator == '+') {
            channel->setInviteOnly(true);
            channel->addMode("i");
            channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "+i"));
        } else if (modeOperator == '-') {
            channel->setInviteOnly(false);
            channel->delMode("i");
            channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "-i"));
        }
    } else if (modeCommand == 't') {
        if (!param.empty()) {
            server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "t", param, "Param is not empty"));
            return;
        }
        if (modeOperator == '+') {
            channel->setTopicRestriction(true);
            channel->addMode("t");
            channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "+t"));
        } else if (modeOperator == '-') {
            channel->setTopicRestriction(false);
            channel->delMode("t");
            channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "-t"));
        }
    } else if (modeCommand == 'k') {
        if (modeOperator == '+') {
            if (param.empty()) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "+k", param, "Key is empty"));
                return;
            } else {
                channel->setHavePassword(true);
                channel->setPassword(param);
                channel->addMode("k");
                channel->sendMessageToAll(MODE_CHANNELMSGWITHPARAM(channelName, "+k", param));
            }
        } else if (modeOperator == '-') {
            std::string password = server->getPassword();
            if (param != password) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "-k", param, "Key is invalid"));
                return;
            } else {
                channel->setHavePassword(false);
                channel->setPassword("");
                channel->delMode("k");
                channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "-k"));
            }
        }
    } else if (modeCommand == 'o') {
        if (modeOperator == '+') {
            if (param.empty()) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "+o", param, "User is empty"));
                return;
            } else {
                Client *target = channel->getUserByNickName(param);
                if (!target) {
                    server->sendData(client, ERR_USERNOTINCHANNEL(srcNick, param, channelName));
                    return;
                }
                channel->addAdmins(target);
                channel->sendMessageToAll(MODE_CHANNELMSGWITHPARAM(channelName, "+o", param));
            }
        } else if (modeOperator == '-') {
            if (param.empty()) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "-o", param, "User is empty"));
                return;
            } else {
                Client *target = channel->getUserByNickName(param);
                if (!target) {
                    server->sendData(client, ERR_USERNOTINCHANNEL(srcNick, param, channelName));
                    return;
                }
                channel->delAdmin(target);
                channel->sendMessageToAll(MODE_CHANNELMSGWITHPARAM(channelName, "-o", param));
            }
        }
    } else if (modeCommand == 'l') {
        if (modeOperator == '+') {
            if (!strContainOnlyDigits(param)) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "+l", param, "Limit is not a number"));
                return;
            }
            int maxUsers = std::atoi(param.c_str());
            channel->setHaveMaxUsers(true);
            channel->setMaxUsers(maxUsers);
            channel->addMode("l");
            channel->sendMessageToAll(MODE_CHANNELMSGWITHPARAM(channelName, "+l", param));
        } else if (modeOperator == '-') {
            if (!param.empty()) {
                server->sendData(client, ERR_INVALIDMODEPARAM(srcNick, channelName, "-l", param, "Limit is not empty"));
                return;
            }
            channel->setHaveMaxUsers(false);
            channel->setMaxUsers(std::numeric_limits<int>::max());
            channel->delMode("l");
            channel->sendMessageToAll(MODE_CHANNELMSG(channelName, "-l"));
        }
    } else {
        server->sendData(client, ERR_UMODEUNKNOWNFLAG(srcNick));
    }
}
