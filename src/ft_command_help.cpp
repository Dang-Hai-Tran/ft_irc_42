/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_help.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:01:55 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 19:01:57 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_infor_username(Client* client)
{
	ft_send(client, "* Command: /USER username mode unused :realname");
	ft_send(client, "Use: is used to set the user and real name information for your connection to the IRC server");
	ft_send(client, "Mode: 0 (normal) - 8(invisible)");
	ft_send(client, "Unused: *");
}

void	ft_infor_nickname(Client* client)
{
	ft_send(client, "* Command: /NICK nickname");
	ft_send(client, "Use: is used to change your nickname");
}

void	ft_infor_privmsg(Client* client)
{
	ft_send(client, "* Command: /PRIVMSG user, #channel :messages");
	ft_send(client, "Use: send private messages to user or channel");
}

/**************************************************************/

void	ft_infor_(Client* client)
{
	ft_send(client, "* Command: ");
	ft_send(client, "Use: ");
}

void	ft_infor_help(Client* client)
{
	ft_send(client, "* Command: /HELP");
	ft_send(client, "Use: displays all valid commands on the server");
}

void	ft_infor_password(Client* client)
{
	ft_send(client, "* Command: /PASS server_password");
	ft_send(client, "Use: login to the IRC server");
}

void	ft_infor_login(Client* client)
{
	ft_send(client, "* Command: /LOGIN username");
	ft_send(client, "Use: login to the IRC server");
}

void	ft_infor_who(Client* client)
{
	ft_send(client, "* Command: /WHO");
	ft_send(client, "Use: shows all clients connected to the server");
}

void	ft_infor_whois(Client* client)
{
	ft_send(client, "* Command: /WHOIS nickname");
	ft_send(client, "Use: is used to retrieve detailed information about a specific user on the network");
}

/**************************************************************/

void	ft_command_help(Client* client)
{
	if (client->m_getParameter() != "")
		return (error_syntax(client));

	ft_infor_help(client);
	ft_send(client, "   ---------------------------------------   ");

	if (client->m_isConnected() == false)
	{
		ft_infor_password(client);
		ft_send(client, "   ---------------------------------------   ");
		return ;
	}

	if (client->m_getStatusS() == false)
	{
		ft_infor_login(client);
		ft_send(client, "   ---------------------------------------   ");
	}

	ft_infor_username(client);
	ft_send(client, "   ---------------------------------------   ");

	if (client->m_getUserName() == "")
		return ;

	ft_infor_nickname(client);
	ft_send(client, "   ---------------------------------------   ");

	if (client->m_getNickName() == "")
		return ;

	ft_infor_who(client);
	ft_send(client, "   ---------------------------------------   ");
	ft_infor_whois(client);
	ft_send(client, "   ---------------------------------------   ");
	ft_infor_privmsg(client);
	ft_send(client, "   ---------------------------------------   ");
}
