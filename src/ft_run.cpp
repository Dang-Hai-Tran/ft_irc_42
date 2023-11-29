#include "../inc/irc.hpp"

void ft_command_outside(Server &server, Client &client) {
    std::string cmd = client.m_getCmd();

    if (cmd == "HELP")
        ft_command_help(client);
    else if (cmd == "USER")
        ft_command_username(server, client);
    else if (cmd == "WHO")
        ft_command_who(server, client);
    else if (cmd == "WHOIS")
        ft_command_whois(server, client);
    else if (cmd == "NICK")
        ft_command_nickname(server, client);
    else if (cmd == "PRIVMSG")
        ft_command_privmsg(server, client);
    else if (client.m_getStatusC() == false && cmd != "JOIN")
        ft_send(client, 4, "(!) This command is invalid");
}

void reset_data(Client &client) {
    client.m_setSocket(0);
    client.m_setConnected(false);
    client.m_setStatusS(false);
    client.m_setStatusC(false);
    client.m_setInput("");
    client.m_setCmd("");
    client.m_setParameter("");
}

void ft_guide(Client &client) {
    ft_send(client, 3, "(i) Use /HELP for instructions");
    if (client.m_isConnected() == false) {
        ft_send(client, 3, "(i) You need to login with /PASS");
        ft_send(client, 3, "Command: /PASS <password>");
    } else if (client.m_getStatusS() == false) {
        ft_send(client, 3, "(i) Use /USER <username> <mode> * <realname>");
    }
}

void ft_run(Server &server, Client &client) {
    // std::string cmd = client.m_getCmd();

    // if (cmd == "HELP")
    //     ft_command_help(client);
    // else if (client.m_isConnected() == false)
    //     ft_requestPassword(server, client);
    // else if (client.m_getStatusS() == false)
    //     ft_requestUserName(server, client);
    // else if (client.m_getNickName() == "")
    //     ft_requestNickName(server, client);
    // else if (client.m_getStatusC() == true || cmd == "JOIN")
    //     commandChannel(server, client);
    // else
    //     ft_command_outside(server, client);
    // ft_send(client, 1, "\n--------------------------------------------------\n");
    // ft_guide(client);
    commandChannel(server, client);
}
