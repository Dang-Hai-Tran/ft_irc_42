#include "../inc/irc.hpp"

void commandChannel(Server &server, Client &client) {
    std::string cmd = client.m_getCmd();
    std::string message = client.m_getInput();
    trimEndOfLine(message);
    std::cout << "Client fd: " << client.m_getSocket() << " send >> " << message << std::endl;
    if (cmd == "JOIN") {
        commandJoin(&server, &client, message);
    }
    if (cmd == "PART") {
        commandPart(&server, &client, message);
    }
    if (cmd == "NAMES") {
        commandNames(&server, &client, message);
    }
    if (cmd == "INVITE") {
        commandInvite(&server, &client, message);
    }
    if (cmd == "KICK") {
        commandKick(&server, &client, message);
    }
    if (cmd == "MODE") {
        commandMode(&server, &client, message);
    }
}
