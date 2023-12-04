/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_connected_successfully.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:13:15 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/04 18:38:16 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	sign_up(Server& server, Client* client)
{
	server.getRegisteredClients().push_back(client);
	client->m_setID(server.getRegisteredClients().size());
	if (client->m_getID() == 1)
		client->m_setAdminServer(true);
	client->m_setStatusS(true);

	// If there are 2 names that are the same
	std::string	nickName = client->m_getNickName();
	int	id = ft_find_nickname(server, nickName);

	if (id != 0 && id != client->m_getID())
	{
		std::string str_id = int_to_string(client->m_getID());
		nickName += str_id;
		client->m_setNickName(nickName);
	}
}
void	sign_in(Server& server, Client* &client, int id)
{
	std::string	nickName = client->m_getNickName();
	std::string	realname = client->m_getRealName();

	Client*	oldClient = server.getRegisteredClients()[id];
	oldClient->m_setNickName(nickName);
	oldClient->m_setRealName(realname);
	oldClient->m_setSocket(client->m_getSocket());
	oldClient->m_setMode(client->m_getMode());
	oldClient->m_setConnected(true);
	oldClient->m_setStatusS(true);

	// std::cout << "1 --> " << oldClient << std::endl;
	delete client;
	client = oldClient;
}

void	connected_successfully(Server& server, Client* &client)
{
	std::string	userName = client->m_getUserName();
	std::string	nickName = client->m_getNickName();
	ft_send(client, "Helloooooo [" + userName + "] !!!");

	ft_send(client, "\n----------------------------------------\n");
	if (client->m_usingIrssi()) // signal connected for IRSSI
	{
		std::string	text = ":localhost 001 " + nickName + " :<--    Welcome to the IRC Network    -->";
		ft_send(client, text);
	}
	else
		ft_send(client, "<--    Welcome to the IRC Network    -->");

	// number connection
	std::string nbr_connection = int_to_string((int)server.m_getListConnection().size());
	ft_send(client, "Number connection: " + nbr_connection);

	// update status
	if (client->m_getStatusS())
		return ;

	int	id = ft_find_username(server, userName);
	if (id == 0)
		sign_up(server, client);
	else
		sign_in(server, client, id - 1);
}
