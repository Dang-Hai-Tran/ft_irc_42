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
    }
    if (cmd == "PART") {
        commandPart(&server, &client, input);
    }
    if (cmd == "NAMES") {
        commandNames(&server, &client, input);
    }
    if (cmd == "TOPIC") {
        commandTopic(&server, &client, input);
    }
    if (cmd == "INVITE") {
        commandInvite(&server, &client, input);
    }
    if (cmd == "KICK") {
        commandKick(&server, &client, input);
    }
    if (cmd == "MODE") {
        commandMode(&server, &client, input);
    }
    if (cmd == "PRIVMSG") // xuluu
        ft_command_privmsg(server, &client);
}
