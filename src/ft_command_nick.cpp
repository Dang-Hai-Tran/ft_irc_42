/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_nick.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:54:57 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/05 21:12:45 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

int	ft_find_username(Server& server, std::string& userName)
{
	int	i(0);
	int	nb_clients = server.getRegisteredClients().size();

	while (i < nb_clients)
	{
		Client*	client = server.getRegisteredClients()[i];
		std::string un = client->m_getUserName();
		if (un == userName)
			return (client->m_getID());
		i++;
	}
	return (0);
}

int	ft_find_nickname(Server& server, std::string& nickName)
{
	int	i(0);
	int	nb_clients = server.getRegisteredClients().size();

	while (i < nb_clients)
	{
		Client*	client = server.getRegisteredClients()[i];
		std::string nn = client->m_getNickName();
		if (nn == nickName)
			return (client->m_getID());
		i++;
	}
	return (0);
}

void	ft_nickname_accepted(Client* client, std::string nickName)
{
	std::string	oldNick = client->m_getNickName();
	client->m_setNickName(nickName);

	if (!client->m_getStatusS())
		ft_send(client, "(i) The nickname can be changed after the username is created because there is a duplicate nick name in the server\r\n");

	std::string	newNick = client->m_getNickName();
	ft_send(client, RPL_NICK(oldNick, newNick, newNick));
	if (!client->m_usingIrssi())
		ft_guide(client);
}

void	ft_command_nick(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();
	std::string	socket = int_to_string(client->m_getSocket());

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	int	id = ft_find_nickname(server, parameter);
	if (id != 0 && id != client->m_getID())
	{
		Client*	user = server.getRegisteredClients()[id - 1];
		if (user->m_getStatusS())
		{
			ft_send(client, ERR_NICKNAMEINUSE(socket, parameter));
			return ;
		}
	}
	ft_nickname_accepted(client, parameter);
}
