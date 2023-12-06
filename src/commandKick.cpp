#include "../inc/irc.hpp"

/**
 * Kicks one or more users from a channel.
 * Syntax: KICK <channel>{,<channel>} <user>{,<user>} [<comment>]
 * Example: KICK #foobar JohnDoe
 *
 * @param server   Pointer to the server object.
 * @param client   Pointer to the client object.
 * @param input    The input string containing the command and arguments.
 */
void commandKick(Server *server, Client *client, std::string input) {
    std::vector<std::string> args = cmdFindArgs(input);
    std::string reason = cmdFindMessage(input);
    if (reason.empty())
        reason = "Kicked by the channel's operator";
    std::string srcNick = client->m_getNickName();
    std::string srcUsername = client->m_getUserName();
    if (args.size() != 3) {
        server->sendData(client, ERR_NEEDMOREPARAMS(srcNick, "KICK"));
        return;
    }
    std::string channelName = args[1];
    Channel *channel = server->getChannel(channelName);
    if (channel == NULL) {
        server->sendData(client, ERR_NOSUCHCHANNEL(srcNick, channelName));
        return;
    }
    // Check if src is on channel
    if (!channel->isInChannel(client)) {
        server->sendData(client, ERR_NOTONCHANNEL(srcNick, channelName));
        return;
    }
    // Check if src is admin
    if (!channel->isAdmin(client)) {
        server->sendData(client, ERR_CHANOPRIVSNEEDED(srcNick, channelName));
        return;
    }
    std::vector<std::string> listTargetNick = splitString(args[2], ',');
    for (size_t i = 0; i < listTargetNick.size(); i++) {
        std::string targetNick = listTargetNick[i];
        Client *target = server->getClientByNickName(targetNick);
        if (target == NULL) {
            server->sendData(client, ERR_NOSUCHNICK(srcNick, targetNick));
            continue;
        }
        if (!channel->isInChannel(target)) {
            server->sendData(client, ERR_USERNOTINCHANNEL(srcNick, targetNick, channelName));
            continue;
        }
        // Send RPL_KICK to users on channel (kicked user included)
        channel->sendMessageToAll(RPL_KICK(user_id(srcNick, srcUsername), channelName, targetNick, reason));
        channel->delUser(target);
        target->delChannel(channel);
        if (target->getChannelsUserIn().size() == 0)
            target->m_setStatusC(false);
        if (DEBUG) {
            std::cout << "Address of target: " << target << std::endl;
            std::cout << "Status C of client fd: " << client->m_getSocket() << ", value: " << client->m_getStatusC() << std::endl;
        }
    }
}
