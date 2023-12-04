#include "../inc/irc.hpp"

// PART <channel>{,<channel>} [<reason>]
void commandPart(Server *server, Client *client, std::string input) {
    std::string nick = client->m_getNickName();
    std::string username = client->m_getUserName();
    std::string reason = cmdFindMessage(input);
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() != 2) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "PART"));
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    for (size_t i = 0; i < listChannelName.size(); i++) {
        std::string channelName = listChannelName[i];
        Channel *channel = server->getChannel(channelName);
        // Check if channel exist
        if (channel == NULL) {
            server->sendData(client, ERR_NOSUCHCHANNEL(nick, channelName));
            continue;
        }
        // Check if user in channel
        if (!channel->isInChannel(client)) {
            server->sendData(client, ERR_NOTONCHANNEL(nick, channelName));
            continue;
        }
        channel->delUser(client);
        client->delChannel(channel);
        std::cout << nick << " has left channel " << channelName << std::endl;
        // Send leaving message to all users in channel
        server->sendData(client, RPL_PART(user_id(nick, username), channelName, reason));
        channel->sendMessageToAll(RPL_PART(user_id(nick, username), channelName, reason));
        if (channel->getNumberUsers() == 0) {
            server->delChannel(channel);
        }
    }
    if (client->getChannelsUserIn().size() == 0)
        client->m_setStatusC(false);
}
