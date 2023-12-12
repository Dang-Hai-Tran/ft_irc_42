/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:19:03 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/07 15:48:08 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void ft_command_outside(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();

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
    else if (cmd == "PRIVMSG")
        ft_command_privmsg(server, client);
    else if (cmd == "MODE" && client->m_usingIrssi())
        client->m_setMode(true);
    else if (!client->m_getStatusC())
        ft_send(client, ERR_UNKNOWNCOMMAND(client->m_getNickName(), client->m_getCmd()));
}

void reset_data(Client *client) {
    client->m_setSocket(0);
    client->m_setConnected(false);
    client->m_setStatusS(false);
    client->m_setStatusC(false);
    client->m_setModeClient(false);
    client->m_setInput("");
    client->m_setCmd("");
    client->m_setParameter("");
}

void ft_guide(Client *client) {
    if (client->m_isConnected() == false) {
        ft_send(client, "(!) Usage: /PASS <password> to login server\r\n");
    } else if (client->m_getNickName() == "") {
        ft_send(client, "(!) Usage: /NICK <nickname> to set new nick\r\n");
    } else if (client->m_getUserName() == "") {
        ft_send(client, "(!) Usage: /USER <username> 0 * :<realname> to set new username\r\n");
    }
}

int ft_general_command(Server *server, Client *client) {
    std::string cmd = client->m_getCmd();
    std::string nick = client->m_getNickName();
    std::string username = client->m_getUserName();

    if (cmd == "QUIT")
    {
        if (ft_command_quit(client))
            return (0);
    }
    if (cmd == "HELP")
        ft_command_help(client);
    else if (cmd == "CLEAR")
        ft_command_clear(client);
    else if (cmd == "PING" && client->m_usingIrssi())
        server->sendData(client, RPL_PONG(user_id(nick, username), ""));
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
    } else if (client->m_getStatusC() == true || cmd == "JOIN" || cmd == "BOT" || cmd == "TRANSFER")
        commandChannel(server, *client);
    else
        ft_command_outside(server, client);
    return (1);
}

bool ft_run(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();

    int code = ft_general_command(&server, client);
    if (code == 0)
        return (0);
    else if (code == 1 && !ft_request_informations(server, client))
        return (0);
    return (1);
}
