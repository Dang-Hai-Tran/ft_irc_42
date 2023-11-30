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

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (parameter != server.getPassword())
	{
		ft_send(client, "(!) Wrong password");
		return ;
	}
	ft_send(client, "(✓) Login successful.");
	client->m_setConnected(true);
}
