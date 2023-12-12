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
    std::string socket = int_to_string(client->m_getSocket());
    ft_send(client, ERR_UNKNOWNCOMMAND(socket, client->m_getCmd()));
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
