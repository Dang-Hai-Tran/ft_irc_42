/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_request.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:18:47 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/28 17:18:50 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

bool	ft_requestPassword(Server& server, Client* client)
{
	std::string cmd = client->m_getCmd();

	if (cmd != "PASS")
	{
		ft_send(client, "(i) Use /PASS to access the server");
		ft_send(client, "Command: /PASS server_password");
		return (0);
	}
	ft_command_pass(server, client);
	if (client->m_isConnected() == false)
		return (0);
	return (1);
}

bool	ft_requestNickName(Server& server, Client* client)
{
	std::string cmd = client->m_getCmd();

	if (cmd != "NICK")
	{
		ft_send(client, "(i) Use /NICK to login");
		ft_send(client, "Command: /NICK your_nickname");
		return (0);
	}
	ft_command_nick(server, client);
	if (client->m_getNickName() == "")
		return (0);
	return (1);
}

bool	ft_requestUserName(Server& server, Client* &client)
{
	std::string cmd = client->m_getCmd();

	if (cmd != "USER")
	{
		ft_send(client, "(i) Use /USER to login");
		ft_send(client, "Command: /USER username 0 * :realname");
		return (0);
	}

	// get username
	if (client->m_usingIrssi())
		ft_irssi_get_realname(server, client);
	else
		ft_command_user(server, client);

	if (client->m_getUserName() == "")
		return (0);
	return (1);
}
