/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_nickname.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:54:57 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/17 12:56:24 by xuluu            ###   ########.fr       */
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

void	ft_command_nick(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	int	id = ft_find_nickname(server, parameter);
	if (id != 0 && id != client->m_getID())
	{
		Client*	user = server.getRegisteredClients()[id - 1];
		if (user->m_getStatusS())
		{
			std::string text = ":localhost 433 * " + parameter + " :Nickname is already in use";
			ft_send(client, text);
			return ;
		}
	}
	std::string	oldNick = client->m_getNickName();
	client->m_setNickName(parameter);
	if (client->m_usingIrssi())
	{
		std::string	newNick = client->m_getNickName();
		ft_send(client, ":" + oldNick + " NICK :" + newNick);
	}
	else
	{
		ft_send(client, "You're now know as [" + parameter + "]");
		ft_send(client, "\n-----------------------------------------\n");
		ft_guide(client);
	}
}
