/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:19:03 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 18:04:39 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void ft_command_outside(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();
    // std::cout << cmd << std::endl;

    if (cmd == "HELP")
        ft_command_help(client);
    else if (cmd == "USER")
        ft_command_user(server, client);
    else if (cmd == "WHO")
        ft_command_who(server, client);
    else if (cmd == "WHOIS")
        ft_command_whois(server, client);
    else if (cmd == "NICK")
        ft_command_nick(server, client);
    else if (cmd == "PRIVMSG") // delete
        ft_command_privmsg(server, client);
    else if ((cmd == "MODE" && !client->m_getStatusC()) || cmd == "PING" || cmd == "QUIT")
        return;
    else if (client->m_getStatusC() == false) {
        ft_send(client, "(!) This command is invalid");
        ft_send(client, "(i) Use /HELP for instructions");
    }
}

void reset_data(Client *client) {
    client->m_setSocket(0);
    client->m_setConnected(false);
    client->m_setStatusS(false);
    client->m_setStatusC(false);
    client->m_setInput("");
    client->m_setCmd("");
    client->m_setParameter("");
    client->m_setModeClient(false);
}

void ft_guide(Client *client) {
    if (client->m_isConnected() == false) {
        ft_send(client, "(i) Use /PASS to access the server");
        ft_send(client, "Command: /PASS server_password");
    } else if (client->m_getNickName() == "") {
        ft_send(client, "(i) Use /NICK to login");
        ft_send(client, "Command: /NICK your_nickname");
    } else if (client->m_getUserName() == "") {
        ft_send(client, "(i) Use /USER to login");
        ft_send(client, "Command: /USER username 8 * :realname");
    }
}

int ft_general_command(Client *client) {
    std::string cmd = client->m_getCmd();

    if (cmd == "QUIT")
        return (0);
    if (cmd == "HELP")
        ft_command_help(client);
    else if (cmd == "CLEAR")
        ft_command_clear(client);
    else if (cmd == "PING" && client->m_usingIrssi())
        ft_send(client, "PONG :localhost");
    else
        return (1);
    return (2);
}

bool ft_request_informations(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();

    if (client->m_isConnected() == false) {
        if (!ft_requestPassword(server, client))
            return (0);
    } else if (client->m_getNickName() == "") {
        if (!ft_requestNickName(server, client))
            return (0);
    } else if (client->m_getUserName() == "") {
        if (!ft_requestUserName(server, client))
            return (0);
    } else if (client->m_getStatusC() == true || cmd == "JOIN")
        commandChannel(server, *client);
    else
        ft_command_outside(server, client);
    return (1);
}

bool ft_run(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();

    int code = ft_general_command(client);
    if (code == 0)
        return (0);
    else if (code == 1 && !ft_request_informations(server, client))
        return (0);

    // std::cout << "2 --> " << client << std::endl;
    if (client->m_getStatusS()) {
        if ((cmd == "MODE" && !client->m_getStatusC()) || cmd == "PING")
            return (1);
        ft_send(client, "\n----------------------------------------\n");
        if (!client->m_usingIrssi()) {
            std::string text = "<" + client->m_getNickName() + "> ";
            send(client->m_getSocket(), text.c_str(), text.size(), 0);
        }
    }
    return (1);
}
