/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_help.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:01:55 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/06 15:04:50 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void ft_infor_username(Client *client) {
    ft_send(client, "* Command: /USER username mode unused :realname\r\n");
    ft_send(client, "Use: is used to set the user and real name information for your connection to the IRC server\r\n");
    ft_send(client, "Mode: 0 (normal) - 8(invisible)\r\n");
    ft_send(client, "Unused: *\r\n");
}

void ft_infor_nickname(Client *client) {
    ft_send(client, "* Command: /NICK nickname\r\n");
    ft_send(client, "Use: is used to change your nickname\r\n");
}

/**************************************************************/

void ft_infor_(Client *client) {
    ft_send(client, "* Command: \r\n");
    ft_send(client, "Use: \r\n");
}

void ft_infor_clear(Client *client) {
    ft_send(client, "* Command: /CLEAR\r\n");
    ft_send(client, "Use: clear the windown\r\n");
}

void ft_infor_help(Client *client) {
    ft_send(client, "(!) Usage: /HELP to display all valid commands on server\r\n");
}

void ft_infor_password(Client *client) {
    ft_send(client, "* Command: /PASS server_password\r\n");
    ft_send(client, "Use: login to the IRC server\r\n");
}

void ft_infor_who(Client *client) {
    ft_send(client, "* Command: /WHO\r\n");
    ft_send(client, "Use: shows all clients connected to the server\r\n");
}

void ft_infor_whois(Client *client) {
    ft_send(client, "(!) Usage: /WHOIS <nickname> to retrieve detailed information about a specific user connected to server\r\n");
}

void ft_infor_privmsg(Client *client) {
    ft_send(client, "* Command: /PRIVMSG nickName/#nameChannel :message\r\n");
    ft_send(client, "Use: send a private message for a person or a channel\r\n");
}

/**************************************************************/

void ft_command_help(Client *client) {
    if (client->m_getParameter() != "")
        return (error_syntax(client));

    // general command
    ft_infor_help(client);
    ft_send(client, "   ---------------------------------------   \r\n");
    ft_infor_clear(client);
    ft_send(client, "   ---------------------------------------   \r\n");

    // password
    if (client->m_isConnected() == false) {
        ft_infor_password(client);
        ft_send(client, "   ---------------------------------------   \r\n");
        return;
    }

    // nickname
    ft_infor_nickname(client);
    ft_send(client, "   ---------------------------------------   \r\n");
    if (client->m_getNickName() == "")
        return;

    // username
    ft_infor_username(client);
    ft_send(client, "   ---------------------------------------   \r\n");
    if (client->m_getUserName() == "")
        return;

    ft_infor_privmsg(client);
    ft_send(client, "   ---------------------------------------   \r\n");
    ft_infor_who(client);
    ft_send(client, "   ---------------------------------------   \r\n");
    ft_infor_whois(client);
    ft_send(client, "   ---------------------------------------   \r\n");
}
