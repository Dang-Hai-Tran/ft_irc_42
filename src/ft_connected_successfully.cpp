/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_connected_successfully.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:13:15 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 21:20:47 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void sign_up(Server &server, Client *client) {
    server.getRegisteredClients().push_back(client);
    client->m_setID(server.getRegisteredClients().size());
    if (client->m_getID() == 1)
        client->m_setAdminServer(true);
    client->m_setStatusS(true);

    // If there are 2 names that are the same
    std::string nickName = client->m_getNickName();
    int id = ft_find_nickname(server, nickName);

    if (id != 0 && id != client->m_getID()) {
        std::string str_id = int_to_string(client->m_getID());
        nickName += str_id;
        client->m_setNickName(nickName);
    }
}
void sign_in(Server &server, Client *&client, int id) {
    std::string input = client->m_getInput();
    std::string nickName = client->m_getNickName();
    std::string realName = client->m_getRealName();
    bool typeClient = client->m_usingIrssi();
    bool mode = client->m_isInvisible();
    int socket = client->m_getSocket();

    // update
    Client *oldClient = server.getRegisteredClients()[id];
    oldClient->m_setSocket(socket);
    oldClient->m_setInput(input);
    oldClient->m_setNickName(nickName);
    oldClient->m_setRealName(realName);
    oldClient->m_setModeClient(typeClient);
    oldClient->m_setMode(mode);
    oldClient->m_setConnected(true);
    oldClient->m_setStatusS(true);

    Client*	tmp = client;
    client = oldClient;
    delete	tmp;
    std::cout << "2 --> " << client << std::endl;
}

void	ft_welcome(Client* client)
{
    std::string nickName = client->m_getNickName();
    std::string socket = int_to_string(client->m_getSocket());
    
    ft_send(client, "\n----------------------------------------\r\n");
    ft_send(client, RPL_WELCOME(socket, nickName));
}

void connected_successfully(Server &server, Client *&client) {
    std::string userName = client->m_getUserName();
    std::string nickName = client->m_getNickName();
    
    std::string text = "Hellooooo [" + userName + "]";
    if (client->m_getStatusC())
        ft_send(client, RPL_PRIVMSG2(nickName, "", text));
    else
        ft_send(client, text + "\r\n");
    
    // update status
    if (client->m_getStatusS())
        return;

    // print welcome
    ft_welcome(client);

    int id = ft_find_username(server, userName);
    if (id == 0)
        sign_up(server, client);
    else
        sign_in(server, client, id - 1);

    // number connection
    std::string nbr_connection = int_to_string((int)server.m_getListConnection().size());
    std::string nbrRegisteredUsers = int_to_string((int)server.getRegisteredClients().size());
    std::string nbrChannels = int_to_string((int)server.getChannels().size());
    std::string message = "Actually, the server has " + nbr_connection + " users connected, " + nbrRegisteredUsers + " users registered, and " + nbrChannels + " channels created\r\n";
    ft_send(client, message);
}
