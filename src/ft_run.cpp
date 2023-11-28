/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:19:03 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/28 17:19:05 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_command_outside(Server& server, Client* &client)
{
	std::string	cmd = client->m_getCmd();

	if (cmd == "HELP")
		ft_command_help(client);
	else if (cmd == "USER")
		ft_command_user(server, client);
	else if (cmd == "WHO")
		ft_command_who(server, client);
	else if (cmd == "WHOIS")
		ft_command_whois(server, client);
	else if (cmd == "NICK")
		ft_command_nick(server, client);
	else if (cmd == "PRIVMSG")
		ft_command_privmsg(server, client);
	else if (client->m_getStatusC() == false)
	{
		ft_send(client, "(!) This command is invalid");
		ft_send(client, "(i) Use /HELP for instructions");
	}
}

void	reset_data(Client* client)
{
	client->m_setSocket(0);
	client->m_setConnected(false);
	client->m_setStatusS(false);
	client->m_setStatusC(false);
	client->m_setInput("");
	client->m_setCmd("");
	client->m_setParameter("");
}

void	ft_guide(Client* client)
{
	if (client->m_isConnected() == false)
	{
		ft_send(client, "(i) Use /PASS to access the server");
		ft_send(client, "Command: /PASS server_password");
	}
	else if (client->m_getStatusS() == false)
	{
		ft_send(client, "(i) Use /LOGIN to login");
		ft_send(client, "Command: /LOGIN username");
	}
}

bool	ft_run(Server& server, Client* client)
{
	std::string	cmd = client->m_getCmd();

	if (cmd == "HELP")
		ft_command_help(client);
	else if (client->m_isConnected() == false)
	{
		if (!ft_requestPassword(server, client))
			return (0);
	}
	else if (client->m_getNickName() == "")
	{
		if (!ft_requestNickName(server, client))
			return (0);
	}
	else if (client->m_getStatusS() == false)
	{
		if (!ft_requestUserName(server, client))
			return (0);
	}
	else if (client->m_getStatusC() == false)
		ft_command_outside(server, client);
	else
		std::cout << "Inside a channel !!!" << std::endl;

	std::cout << "2 --> " << client << std::endl;
	ft_send(client, "\n--------------------------------------------------\n");
	ft_guide(client);
	return (1);
}
