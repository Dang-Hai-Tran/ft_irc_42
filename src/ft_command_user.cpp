/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_user.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:54:57 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 13:42:04 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

bool reset_data_username(Client *client) {
    client->m_setUserName("");
    client->m_setMode(false);
    client->m_setRealName("");
    return (0);
}

bool ft_check_username(Server &server, Client *client, std::string userName) {
    int id = ft_find_username(server, userName);

    if (id != 0 && id != client->m_getID()) {
        Client *user = server.getRegisteredClients()[id - 1];
        if (user->m_getStatusS()) {
            std::string text = "";
            if (client->m_usingIrssi())
                text = ":localhost 433 * " + userName + " :Username is already in use";
            else
                text = "(!) Username is already in use";
            std::cout << text << std::endl;
            ft_send(client, text);
            return (reset_data_username(client));
        }
    }
    client->m_setUserName(userName);
    return (1);
}

// 8: invisible, 0: normal
bool ft_check_mode(Client *client, std::string &mode) {
    if (mode.size() != 1 || (mode[0] != '0' && mode[0] != '8'))
        return (reset_data_username(client));

    client->m_setMode(1);
    if (mode == "0")
        client->m_setMode(0);
    return (1);
}

bool ft_check_unuserd(Client *client, std::string &unuserd) {
    if (unuserd != "*")
        return (reset_data_username(client));
    return (1);
}

bool ft_check_realname(Client *client, std::string &realname) {
    if (realname.size() < 2 || realname[0] != ':')
        return (reset_data_username(client));
    realname = realname.substr(1, realname.size());
    client->m_setRealName(realname);
    return (1);
}

std::string get_parameter(std::string &str) {
    int i(0);

    while (str[i] && str[i] != ' ')
        i++;

    std::string new_str = str.substr(0, i);

    if (str[i] == ' ')
        i++;
    str = str.substr(i, str.size());
    return (new_str);
}

void ft_command_user(Server &server, Client *&client) {
    std::string parameter = client->m_getParameter();
    int min_space = 3;

    if (parameter == "" || ft_nbrSpace(parameter) < min_space)
        return (error_syntax(client));

    std::string str = get_parameter(parameter);
    if (ft_check_username(server, client, str) == false)
        return;

    str = get_parameter(parameter);
    if (ft_check_mode(client, str) == false)
        return (error_syntax(client));

    str = get_parameter(parameter);
    if (ft_check_unuserd(client, str) == false)
        return (error_syntax(client));

    if (ft_check_realname(client, parameter) == false)
        return (error_syntax(client));

    connected_successfully(server, client);
}
