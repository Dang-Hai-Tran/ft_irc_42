
#include "../inc/irc.hpp"

/**
 * @brief Handles the TOPIC command.
 *
 * This function is responsible for handling the TOPIC command in the IRC server.
 * It allows a client to set or clear the topic of a channel.
 * Syntax: TOPIC <channel> [<topic>].
 * Example: TOPIC #foobar :Hello world!
 *
 * @param server A pointer to the IRC server.
 * @param client A pointer to the client who issued the command.
 * @param input The input string containing the command and arguments.
 */
void commandTopic(Server *server, Client *client, std::string input) {
    std::string nick = client->m_getNickName();
    std::string topic = cmdFindMessage(input);
    size_t indexComma = input.find(':');
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() != 2) {
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "TOPIC"));
        return;
    }
    std::string channelName = args[1];
    Channel *channel = server->getChannel(channelName);
    // Check if channel exist
    if (channel == NULL) {
        server->sendData(client, ERR_NOSUCHCHANNEL(nick, channelName));
        return;
    }
    // Check if user in channel
    if (!channel->isInChannel(client)) {
        server->sendData(client, ERR_NOTONCHANNEL(nick, channelName));
    }
    if (channel->getTopicRestriction() == true) {
        // Check if user is admin of channel
        if (!channel->isAdmin(client)) {
            server->sendData(client, ERR_CHANOPRIVSNEEDED(nick, channelName));
            return;
        }
    }
    // If topic = "", send to topic to client or clear topic
    // else, set topic of channel to args[2]
    if (topic.empty()) {
        if (indexComma == std::string::npos) {
            channel->setTopic(topic);
            std::cout << nick << " clear topic of channel " << channelName << std::endl;
        }
        channel->sendMessageToAll(RPL_TOPIC(nick, channelName, channel->getTopic()));
    } else {
        channel->setTopic(topic);
        std::cout << nick << " set topic of channel " << channelName << " to " << topic << std::endl;
        channel->sendMessageToAll(RPL_TOPIC(nick, channelName, channel->getTopic()));
    }
}
