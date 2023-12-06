
#include "../inc/irc.hpp"

/**
 * @brief Implements the NAMES command, which allows a client to see all users in a channel.
 * Syntax: NAMES <channel>{,<channel>}.
 * Example: NAMES #foobar.
 *
 * @param server Pointer to the Server object.
 * @param client Pointer to the Client object.
 * @param input The input string containing the command and arguments.
 */
void commandNames(Server *server, Client *client, std::string input) {
    std::string nick = client->m_getNickName();
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() != 2) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "NAMES"));
        return;
    }
    std::vector<std::string> listChannelName = splitString(args[1], ',');
    for (size_t i = 0; i < listChannelName.size(); i++) {
        std::string channelName = listChannelName[i];
        Channel *channel = server->getChannel(channelName);
        if (channel == NULL) {
            server->sendData(client, ERR_NOSUCHCHANNEL(nick, channelName));
            server->sendData(client, RPL_ENDOFNAMES(nick, channelName));
            continue;
        }
        // Check if user is in channel
        if (!channel->isInChannel(client)) {
            server->sendData(client, RPL_ENDOFNAMES(nick, channelName));
            continue;
        }
        std::string listUsers = channel->getListOfMembers();
        std::string symbol = "="; // public channel
        server->sendData(client, RPL_NAMREPLY(nick, symbol, channelName, listUsers));
        server->sendData(client, RPL_ENDOFNAMES(nick, channelName));
        std::cout << nick << " get list of users on channel " << channelName << std::endl;
    }
}
