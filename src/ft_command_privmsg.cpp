/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_privmsg.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:14 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 19:02:16 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_conversation(Server& server, Client* client, int start, int id)
{
	std::string	parameter = client->m_getParameter();
	std::string	message = parameter.substr(start , parameter.size());

	if (message.size() < 2 || message[0] != ':')
		return (error_syntax(client));
	message = parameter.substr(start + 1, parameter.size());

	std::string	sender = client->m_getNickName();
	Client* recipient = server.getRegisteredClients()[id];

	ft_send(recipient, "");
	ft_send(recipient, "(!) You have a message from: " + sender);
	ft_send(recipient, "[" + sender + "]: ");
	ft_send(recipient, message + "\n");
}

void	ft_command_privmsg(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) < 1)
		return (error_syntax(client));

	int	i(0);
	while (parameter[i] && parameter[i] != ' ')
		i++;

	std::string	nickName = parameter.substr(0, i);
	int	id = ft_find_nickname(server, nickName);
	if (id == 0)
	{
		ft_send(client, "(!) User does not exist");
		return ;
	}
	else if (id == client->m_getID())
		return ;
	ft_conversation(server, client, i + 1, id - 1);
}
