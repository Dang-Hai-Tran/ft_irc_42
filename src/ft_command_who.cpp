/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_who.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:43 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 19:02:45 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_command_who(Server& server, Client* client)
{
	int	i(0);
	int	nb_clients = server.getRegisteredClients().size();
	std::string	parameter = client->m_getParameter();

	if (parameter != "")
		return (error_syntax(client));

	ft_send(client, "[   Number clients on Server: " + int_to_string(nb_clients) + "   ]");
	while (i < nb_clients)
	{
		Client* user = server.getRegisteredClients()[i];

		std::string id = "[ " + int_to_string(user->m_getID()) + " | ";
		std::string	status = "OFF ]: ";
		if (user->m_getStatusS())
			status = "ON ]: ";
		std::string	nickName = user->m_getNickName() + " ";
		std::string	realName = "(" + user->m_getRealName() + ")";
		
		std::string	text = id + status + nickName + realName;
		ft_send(client, text);
		i++;
	}
}
