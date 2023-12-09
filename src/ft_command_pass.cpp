/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_pass.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:17:38 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/28 17:17:40 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_command_pass(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();
	std::string	socketClient = int_to_string(client->m_getSocket());

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (parameter != server.getPassword())
	{
		ft_send(client, ERR_PASSWDMISMATCH(socketClient));
		return ;
	}

	ft_send(client, "Password correct.\r\n");
	client->m_setConnected(true);

	if (client->m_usingIrssi())
		return ;
	ft_send(client, "\n-----------------------------------------\r\n");
	ft_guide(client);
}
