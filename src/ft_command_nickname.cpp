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

bool	ft_nickName_exist(Server & server, Client & client, const std::string & nickName)
{
	int	i(0);
	int	nb_clients = server.m_getNbrClients();
	int	id = client.m_getID();

	while (i < nb_clients)
	{
		std::string	nn = server.m_client[i].m_getNickName();
		if (nn == nickName)
		{
			if (i == id - 1)
				return (0);
			ft_send(client, 4, "(!) NICKNAME: [" + nickName + "] has been used");
			return (1);
		}
		i++;
	}
	return (0);
}

void	ft_command_nickname(Server& server, Client& client)
{
	std::string	parameter = client.m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (ft_nickName_exist(server, client, parameter))
		return ;
	
	client.m_setNickName(parameter);
	ft_send(client, 4, "(✓) You're now know as [" + parameter + "]");
}
