/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:39:21 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 15:40:03 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

int	ft_nbrNewLine(std::string& str)
{
	int	i(0);
	int	nbr_newline(0);

	while (str[i])
	{
		if (str[i] == '\n')
			nbr_newline++;
		i++;
	}
	return (nbr_newline);
}

void	ft_connection_with_nc(Server& server, Client* client, std::string& cmd)
{
	if (cmd[0] == '/' && cmd[1])
	{
		cmd = ft_delete_space(cmd);
		client->m_setInput(cmd);
		get_command_parameter(server, client);
	}
	else if (client->m_getStatusC() == false)
	{
		ft_send(client, 4, "(!) Command start with '/'");
		ft_send(client, 4, "(i) Use /HELP for instructions");
		ft_send(client, 1, "\n--------------------------------------------------\n");
	}
	else
	{
		ft_send(client, 4, "[" + client->m_getNickName() + "]");
		ft_send(client, 4, cmd);
	}
}

void	get_input(Server& server, Client* client)
{
	std::string str = client->m_getInput();

	if (ft_isStringEmpty(str) || str[0] == '\n')
		return ;

	int	i(0);
	int	nbr_command = ft_nbrNewLine(str);
	
	while (i < nbr_command)
	{
		int	m(0);
		while (str[m] && str[m] != '\n')
			m++;

		std::string	cmd = str.substr(0, m);
		// std::cout << cmd << std::endl;

		// using IRSSI
		if (cmd == "CAP LS\r")
			client->m_setModeClient(true);
		else if (client->m_usingIrssi())
		{
			cmd = str.substr(0, cmd.size() - 1);
			client->m_setInput(cmd);
			ft_irssi_get_input(server, client);
		}
		else
		{
			ft_connection_with_nc(server, client, cmd);
		}

		// Wrong password
		// if (client->m_isConnected() == false)
		// 	return ;

		str = str.substr(m + 1, str.size());
		i++;
	}
}
