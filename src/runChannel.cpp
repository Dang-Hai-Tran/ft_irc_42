#include "../inc/irc.hpp"

void commandChannel(Server &server, Client &client) {
    std::string cmd = client.m_getCmd();
    std::string input = ft_delete_space(client.m_getInput());
    if (DEBUG) {
        std::vector<std::string> args = cmdFindArgs(input);
        std::string message = cmdFindMessage(input);
        std::cout << "Part of args: " << args << std::endl;
        std::cout << "Part of message: " << message << std::endl;
    }
    if (cmd == "JOIN") {
        commandJoin(&server, &client, input);
        return;
    }
    if (cmd == "PART") {
        commandPart(&server, &client, input);
        return;
    }
    if (cmd == "NAMES") {
        commandNames(&server, &client, input);
        return;
    }
    if (cmd == "TOPIC") {
        commandTopic(&server, &client, input);
        return;
    }
    if (cmd == "INVITE") {
        commandInvite(&server, &client, input);
        return;
    }
    if (cmd == "KICK") {
        commandKick(&server, &client, input);
        return;
    }
    if (cmd == "MODE") {
        commandMode(&server, &client, input);
        return;
    }
    if (cmd == "BOT") {
        commandBot(&server, &client, input);
        return;
    }
    if (cmd == "TRANSFER") {
        commandTransfer(&server, &client, input);
        return;
    }
    if (cmd == "PRIVMSG") {
        client.m_defineMessage(true);
        ft_command_privmsg(server, &client);
    }
    else {
        Client *tmp = &client;
        ft_command_outside(server, tmp);
    }
}
