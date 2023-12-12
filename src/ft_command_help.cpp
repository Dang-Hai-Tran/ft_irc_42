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
    ft_send(client, "(!) Usage: /USER <username> <mode(0/8)> * <:realname> to set the user and real name information for your connection to the IRC server\r\n");
}

void ft_infor_nickname(Client *client) {
    ft_send(client, "(!) Usage: /NICK <nickname> to change your nickname\r\n");
}

/**************************************************************/

void ft_infor_(Client *client) {
    ft_send(client, "(!) Usage: \r\n");
}

void ft_infor_clear(Client *client) {
    ft_send(client, "(!) Usage: /CLEAR to clear the windown\r\n");
}

void ft_infor_help(Client *client) {
    ft_send(client, "(!) Usage: /HELP to display all valid commands on server\r\n");
}

void ft_infor_password(Client *client) {
    ft_send(client, "(!) Usage: /PASS <server_password> to login to the IRC server\r\n");
}

void ft_infor_who(Client *client) {
    ft_send(client, "(!) Usage: /WHO to shows all clients connected to the server\r\n");
}

void ft_infor_whois(Client *client) {
    ft_send(client, "(!) Usage: /WHOIS <nickname> to retrieve detailed information about a specific user connected to server\r\n");
}

void ft_infor_privmsg(Client *client) {
    ft_send(client, "(!) Usage: /PRIVMSG <nickName/#nameChannel> <:message> to send a private message for a person or a channel\r\n");
}

/**************************************************************/
void ft_infor_join(Client *client) {
    ft_send(client, "(!) Usage: /JOIN <channel>{,<channel>} [<key>{,<key>}] to join a channel\r\n");
}

void ft_infor_kick(Client *client) {
    ft_send(client, "(!) Usage: /KICK <channel>{,<channel>} <user>{,<user>} [<:comment>] to kick a person out of channel\r\n");
}

void ft_infor_mode(Client *client) {
    ft_send(client, "(!) Usage: /MODE <channel> <mode> <arg>... to set mode for channel\r\n");
}

void ft_infor_names(Client *client) {
    ft_send(client, "(!) Usage: /NAMES <channel>{,<channel>} to see all users in a channel\r\n");
}

void ft_infor_part(Client *client) {
    ft_send(client, "(!) Usage: /PART <channel>{,<channel>} [<:reason>] to leave one or multiple channels\r\n");
}

void ft_infor_topic(Client *client) {
    ft_send(client, "(!) Usage: /TOPIC <channel> [<:topic>] to set or clear the topic of a channel\r\n");
}

void ft_infor_transfer(Client *client) {
    ft_send(client, "(!) Usage: /TRANSFER <filename> <target> to transfer a file from client to client\r\n");
}

void ft_infor_bot(Client *client) {
    ft_send(client, "(!) Usage: /BOT <:message> to ask bot any things\r\n");
}

/**************************************************************/

void ft_command_help(Client *client) {
    if (client->m_getParameter() != "")
        return (error_syntax(client));

    // general command
    ft_infor_help(client);
    ft_infor_clear(client);

    // password
    if (client->m_isConnected() == false) {
        ft_infor_password(client);
        return;
    }

    // nickname
    ft_infor_nickname(client);
    if (client->m_getNickName() == "")
        return;

    // username
    ft_infor_username(client);
    if (client->m_getUserName() == "")
        return;

    ft_infor_privmsg(client);
    ft_infor_who(client);
    ft_infor_whois(client);

    ft_infor_join(client);
    ft_infor_kick(client);
    ft_infor_mode(client);
    ft_infor_names(client);
    ft_infor_part(client);
    ft_infor_topic(client);
    ft_infor_transfer(client);
    ft_infor_bot(client);
}
