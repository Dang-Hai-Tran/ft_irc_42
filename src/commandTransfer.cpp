#include "irc.hpp"
#include <fstream>
#include <sstream>

// Write code to transfer file from client to client
// TRANSFER <filename> <target>
void commandTransfer(Server *server, Client *client, std::string input) {
    std::string nick = client->m_getNickName();
    std::string username = client->m_getUserName();
    std::vector<std::string> args = cmdFindArgs(input);
    if (args.size() != 3) {
        std::cerr << "Error: Wrong number of arguments." << std::endl;
        server->sendData(client, ERR_NEEDMOREPARAMS(nick, "TRANSFER"));
        return;
    }
    std::string filename = args[1];
    std::string targetNick = args[2];
    std::string path = filename;

    // Check if the file exists
    std::ifstream infile(path.c_str());
    if (!infile) {
        std::cerr << "Error: File does not exist." << std::endl;
        server->sendData(client, "Error: File does not exist.");
        return;
    }

    // Read the file into a string
    std::stringstream buffer;
    buffer << infile.rdbuf();
    std::string fileContents = buffer.str();

    // Find target by nickname
    Client *target = server->getClientByNickName(targetNick);
    if (target == NULL) {
        std::cerr << "Error: Target client does not exist." << std::endl;
        server->sendData(client, ERR_NOSUCHNICK(nick, targetNick));
        return;
    }

    // Send a DCC SEND command to the target client
    std::string dccCommand = ":" + nick + "!" + username + "@localhost PRIVMSG " + targetNick + " :\x01 DCC SEND " + filename + " " + fileContents + "\x01";
    server->sendData(target, dccCommand);
}
