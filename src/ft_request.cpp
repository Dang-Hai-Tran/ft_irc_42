/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_request.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:18:47 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 18:05:59 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

bool ft_connection_failed(Client *client) {
    std::string text = "";
    if (client->m_usingIrssi())
        text = ":localhost 421 * :(!) Cannot connect to server (Connection timed out)";
    else
        text = "(!) Cannot connect to server (Connection timed out)";
    ft_send(client, text);
    return (0);
}

bool ft_requestPassword(Server &server, Client *client) {
    std::string cmd = client->m_getCmd();
    std::string password = client->m_getParameter();

    if (cmd != "PASS")
        return (ft_connection_failed(client));
    ft_command_pass(server, client);
    if (client->m_isConnected())
        return (1);
    if (client->m_usingIrssi())
        ft_send(client, ":localhost 464 * :Password incorrect");
    return (0);
}

bool ft_requestNickName(Server &server, Client *client) {
    std::string cmd = client->m_getCmd();

    if (cmd != "NICK")
        return (ft_connection_failed(client));
    ft_command_nick(server, client);
    if (client->m_getNickName() == "")
        return (0);
    return (1);
}

bool ft_requestUserName(Server &server, Client *&client) {
    std::string cmd = client->m_getCmd();

    if (cmd != "USER")
        return (ft_connection_failed(client));

    // get username
    if (client->m_usingIrssi())
        ft_irssi_get_realname(server, client);
    else
        ft_command_user(server, client);

    if (client->m_getUserName() == "")
        return (0);
    return (1);
}
