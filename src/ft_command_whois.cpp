/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_whois.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:52 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/04 18:35:15 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	print_type_client(Client* client, Client* user)
{
	if (user->m_usingIrssi())
		ft_message(client, "* USING: irssi");
	else
		ft_message(client, "* USING: nc");
}

void	print_adminServer(Client* client, Client* user)
{
	if (user->m_getAdminServer())
		ft_message(client, "* ADMIN SERVER: true");
	else
		ft_message(client, "* ADMIN SERVER: false");
}

void	print_adminChannel(Client* client, Client* user)
{
	if (user->m_getAdminChannel())
		ft_message(client, "* ADMIN CHANNEL: true");
	else
		ft_message(client, "* ADMIN CHANNEL: false");
}

void	print_statusS(Client* client, Client* user)
{
	if (user->m_getStatusS())
		ft_message(client, "* STATUS ON SERVER: on");
	else
		ft_message(client, "* STATUS ON SERVER: off");
}

void	print_statusC(Client* client, Client* user)
{
	if (user->m_getStatusC())
		ft_message(client, "* STATUS ON CHANNEL: on");
	else
		ft_message(client, "* STATUS ON CHANNEL: off");
}

void	print_mode(Client* client, Client* user)
{
	if (user->m_isInvisible())
		ft_message(client, "* MODE: invisible");
	else
		ft_message(client, "* MODE: normal");
}

void	ft_display_informations(Server& server, Client* client, int id)
{
	Client*	user = server.getRegisteredClients()[id];
	std::string	numberID = int_to_string(user->m_getID());
	std::string	userName = user->m_getUserName();
	std::string	realName = user->m_getRealName();
	std::string	nickName = user->m_getNickName();

	// print
	ft_message(client, "* USERNAME: " + userName); // delete
	ft_message(client, "* ID: " + numberID);
	ft_message(client, "* NICKNAME: " + nickName);
	ft_message(client, "* REALNAME: " + realName);

	print_type_client(client, user);
	print_mode(client, user);
	print_adminServer(client, user);
	print_adminChannel(client, user);
	print_statusS(client, user);
	print_statusC(client, user);
	// ft_send(client, "*" + );
}

void	ft_command_whois(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (parameter == "")
		parameter = client->m_getNickName();

	int	id = ft_find_nickname(server, parameter);
	if (id == 0 || (server.getRegisteredClients()[id - 1]->m_isInvisible() && server.getRegisteredClients()[id - 1]->m_getID() != client->m_getID()))
	{
		ft_send(client, ERR_NOSUCHNICK(client->m_getNickName(), parameter));
		return ;
	}
	ft_display_informations(server, client, id - 1);
}
