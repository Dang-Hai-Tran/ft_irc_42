/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_clear.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 11:40:28 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/02 11:43:07 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_command_clear(Client* client)
{
	std::string	parameter = client->m_getParameter();
	if (parameter != "")
		return (error_syntax(client));

	int	max_line(100);
	int	i(0);

	while (i < max_line)
	{
		ft_send(client, "\n");
		i++;
	}
}
