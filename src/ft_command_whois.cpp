/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_whois.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:52 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 19:02:54 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	print_adminServer(Client* client, Client* user)
{
	if (user->m_getAdminServer())
		ft_send(client, 4, "* ADMIN SERVER: true");
	else
		ft_send(client, 4, "* ADMIN SERVER: false");
}

void	print_adminChannel(Client* client, Client* user)
{
	if (user->m_getAdminChannel())
		ft_send(client, 4, "* ADMIN CHANNEL: true");
	else
		ft_send(client, 4, "* ADMIN CHANNEL: false");
}

void	print_statusS(Client* client, Client* user)
{
	if (user->m_getStatusS())
		ft_send(client, 4, "* STATUS ON SERVER: on");
	else
		ft_send(client, 4, "* STATUS ON SERVER: off");
}

void	print_statusC(Client* client, Client* user)
{
	if (user->m_getStatusC())
		ft_send(client, 4, "* STATUS ON CHANNEL: on");
	else
		ft_send(client, 4, "* STATUS ON CHANNEL: off");
}

void	print_mode(Client* client, Client* user)
{
	if (user->m_getMode())
		ft_send(client, 4, "* MODE: invisible");
	else
		ft_send(client, 4, "* MODE: normal");
}

void	ft_display_informations(Server& server, Client* client, int id)
{
	Client*	user = server.m_getListConnection()[id];
	std::string	numberID = int_to_string(user->m_getID());
	std::string	userName = user->m_getUserName();
	std::string	realName = user->m_getRealName();
	std::string	nickName = user->m_getNickName();

	// print
	ft_send(client, 4, "* USERNAME: " + userName); // delete
	ft_send(client, 4, "* ID: " + numberID);
	ft_send(client, 4, "* NICKNAME: " + nickName);
	ft_send(client, 4, "* REALNAME: " + realName);

	print_mode(client, user);
	print_adminServer(client, user);
	print_adminChannel(client, user);
	print_statusS(client, user);
	print_statusC(client, user);
	// ft_send(client, 4, "*" + );
}

void	ft_command_whois(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (is_adminServer(client) == false)
		return ;

	int	id = ft_find_nickname(server, parameter);
	if (id == 0)
	{
		ft_send(client, 4, "(!) User does not exist");
		return ;
	}
	ft_display_informations(server, client, id - 1);
}
