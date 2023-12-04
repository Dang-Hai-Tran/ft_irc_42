#include "../inc/irc.hpp"

// JOIN <channel>{,<channel>} [<key>{,<key>}]
void commandJoin(Server *server, Client *client, std::string input) {
    if (DEBUG) {
        std::cout << "Address of client in command join: " << client << std::endl;
    }
    std::vector<std::string> args = splitString(input, ' ');
    std::string nick = client->m_getNickName();
    std::string username = client->m_getUserName();
    if (args.size() < 2 || args.size() > 3) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "JOIN"));
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    std::vector<std::string> listPassword;
    if (args.size() == 3)
        listPassword = splitString(args[2], ',');
    if (!listPassword.empty() && listChannelName.size() != listPassword.size()) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "JOIN"));
        return;
    }
    for (size_t i = 0; i < listChannelName.size(); i++) {
        std::string channelName = listChannelName[i];
        std::string channelPassword = args.size() == 2 ? "" : listPassword[i];
        Channel *channel = server->getChannel(channelName);
        if (channel == NULL) {
            channel = new Channel(server, channelName, channelPassword);
            server->addChannel(channel);
        }
        // Check if user is already in channel
        if (channel->isInChannel(client)) {
            server->sendData(client, ERR_USERONCHANNEL(nick, "", channelName));
            continue;
        }
        // Check if channel is invite only, user is invited
        if (channel->getInviteOnly() && !channel->isInvited(client)) {
            server->sendData(client, ERR_INVITEONLYCHAN(nick, channelName));
            continue;
        }
        // Check if password is correct
        if (channel->getHavePassword() && (listPassword.empty() || channel->getPassword() != listPassword[i])) {
            server->sendData(client, ERR_BADCHANNELKEY(nick, channelName));
            continue;
        }
        // Check if channel have full users
        if (channel->getHaveMaxUsers() && (int)channel->getNumberUsers() >= channel->getMaxUsers()) {
            server->sendData(client, ERR_CHANNELISFULL(nick, channelName));
            continue;
        }
        channel->addUser(client);
        client->addChannel(channel);
        if (client->m_getStatusC() == false)
            client->m_setStatusC(true);
        if (channel->getNumberUsers() == 1)
            channel->addAdmins(client);
        if (channel->isInvited(client))
            channel->delFromInvited(client);
        std::cout << nick << " has joined channel " << channelName << std::endl;
        // Send join message to all users in channel
        channel->sendMessageToAll(RPL_JOIN(user_id(nick, username), channelName));
        // Send topic to user joining channel
        if (channel->getTopic() != "")
            channel->sendMessageToAll(RPL_TOPIC(nick, channelName, channel->getTopic()));
        std::string listOfMembers = channel->getListOfMembers();
        // Depend on channel mode set symbol = '@' if mode = 's', symbol = '*' if mode = 'p', symbol = '=' other cases
        // For simple set symbol = "="
        std::string symbol = "=";
        channel->sendMessageToAll(RPL_NAMREPLY(nick, symbol, channelName, listOfMembers));
        channel->sendMessageToAll(RPL_ENDOFNAMES(nick, channelName));
    }
}
