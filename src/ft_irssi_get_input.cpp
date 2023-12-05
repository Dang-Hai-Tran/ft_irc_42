/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irssi_get_input.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:59:43 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/27 15:02:52 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_irssi_get_username(Server& server, Client* &client)
{
	std::string	parameter = client->m_getParameter();
	int	i(0);

	while (parameter[i] && parameter[i] != ' ')
		i++;
	std::string	userName = parameter.substr(0, i);
	if (ft_check_username(server, client, userName))
		connected_successfully(server, client);
}

void	ft_irssi_get_realname(Server& server, Client* &client)
{
	std::string	parameter = client->m_getParameter();

	size_t	i = parameter.size() - 1;

	while (i > 0 && parameter[i] != ':')
		i--;
	std::string	realName = parameter.substr(i + 1, parameter.size());
	client->m_setRealName(realName);
	
	parameter = parameter.substr(0, i - 1);
	client->m_setParameter(parameter);
	ft_irssi_get_username(server, client);
}
