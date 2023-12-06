#include "../inc/irc.hpp"

/**
 * @file commandInvite.cpp
 * @brief Implementation of the INVITE command.
 * 
 * The INVITE command is used to invite a user to a channel.
 * 
 * Syntax: INVITE <nickname> <channel>.
 * 
 * Example usage: INVITE JohnDoe #general.
 * 
 * If the command is successful, the server sends RPL_INVITING to the user who issued the command
 * and RPL_INVITE to the target user.
 * 
 * If the command fails, the server sends an appropriate error message to the user who issued the command.
 * 
 * @param server Pointer to the Server object.
 * @param client Pointer to the Client object who issued the command.
 * @param input The input string containing the command and its arguments.
 */
void commandInvite(Server *server, Client *client, std::string input) {
    std::string srcNick = client->m_getNickName();
    std::string srcUsername = client->m_getUserName();
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() != 3) {
        server->sendData(client, ERR_NEEDMOREPARAMS(srcNick, "INVITE"));
        return;
    }
    // Find target user on server by nickname
    Client *target = NULL;
    std::string targetNick = args[1];
    for (size_t i = 0; i < server->getOnServerClients().size(); i++) {
        if (server->getOnServerClients()[i]->m_getNickName() == targetNick) {
            target = server->getOnServerClients()[i];
            break;
        }
    }
    if (target == NULL) {
        server->sendData(client, ERR_NOSUCHNICK(srcNick, targetNick));
        return;
    }
    std::string channelName = args[2];
    Channel *channel = server->getChannel(channelName);
    if (channel == NULL) {
        server->sendData(client, ERR_NOSUCHCHANNEL(srcNick, channelName));
        return;
    }
    // Check if user is on channel
    if (!channel->isInChannel(client)) {
        server->sendData(client, ERR_NOTONCHANNEL(srcNick, channelName));
        return;
    }
    // Check if target is on channel
    if (channel->isInChannel(target)) {
        server->sendData(client, ERR_USERONCHANNEL(srcNick, targetNick, channelName));
        return;
    }
    // Send RPL_INVITING to user and RPL_INVITE to target
    server->sendData(client, RPL_INVITING(user_id(srcNick, srcUsername), srcUsername, srcNick, channelName));
    server->sendData(target, RPL_INVITE(user_id(srcNick, srcUsername), targetNick, channelName));
    if (!channel->isInvited(target))
        channel->addToInvited(target);
}
