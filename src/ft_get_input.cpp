/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:39:21 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 22:00:00 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

int ft_nbrNewLine(std::string &str) {
    int i(0);
    int nbr_newline(0);

    while (str[i]) {
        if (str[i] == '\n')
            nbr_newline++;
        i++;
    }
    return (nbr_newline);
}

bool ft_connection_with_nc(Server &server, Client *&client, std::string &cmd) {
    bool run = true;

    if (cmd[0] == '/' && cmd[1]) {
        cmd = ft_delete_space(cmd);
        cmd = cmd.substr(1, cmd.size());
        client->m_setInput(cmd);
        run = get_command_parameter(server, client);
    } else if (client->m_getStatusC()) {
        client->m_defineMessage(true);
        ft_message(client, cmd);
    } else {
        ft_send(client, "(!) Command start with '/'");
        ft_send(client, "(i) Use /HELP for instructions");
        ft_send(client, "\n-------------------------------------------------\n");
        run = client->m_getStatusS();
    }

    if (run) {
        if (client->m_getStatusC())
            return (1);
        std::string text = "";
        text += "[" + client->m_getNickName();
        for (size_t i = 0; i < client->getChannelsUserIn().size(); i++) {
            Channel *channel = client->getChannelsUserIn()[i];
            text += " " + channel->getNameChannel();
        }
        text += "] ";
        if (client->m_getNickName() == "")
            text = "[irc] ";
        send(client->m_getSocket(), text.c_str(), text.size(), 0);
        return (1);
    }
    server.delClientSocket(client->m_getSocket());
    return (0);
}

bool ft_connection_with_irssi(Server &server, Client *&client, std::string &cmd) {
    cmd = cmd.substr(0, cmd.size() - 1);
    client->m_setInput(cmd);

    if (get_command_parameter(server, client))
        return (1);
    server.delClientSocket(client->m_getSocket());
    return (0);
}

bool ft_check_type_client(Server &server, Client *&client, std::string cmd) {
    // using IRSSI
    if (cmd == "CAP LS\r")
        client->m_setModeClient(true);
    else if (client->m_usingIrssi()) {
        if (!ft_connection_with_irssi(server, client, cmd))
            return (0);
    } else {
        if (!ft_connection_with_nc(server, client, cmd))
            return (0);
    }
    return (1);
}

void get_input(Server &server, Client *&client) {
    std::string str = client->m_getInput();

    if (ft_isStringEmpty(str) || str[0] == '\n')
        return;

    int i(0);
    int nbr_command = ft_nbrNewLine(str);

    while (i < nbr_command) {
        int m(0);
        while (str[m] && str[m] != '\n')
            m++;

        std::string cmd = str.substr(0, m);
        // std::cout << cmd << std::endl;

        client->m_defineMessage(false);
        if (!ft_check_type_client(server, client, cmd))
            break;
        str = str.substr(m + 1, str.size());
        i++;
    }
    // std::cout << "3 --> " << client << std::endl;
}
