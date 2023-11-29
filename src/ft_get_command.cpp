/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:03:06 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/20 19:03:09 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_send(Client* client, std::string output)
{
	int	clientSocket = client->m_getSocket();
	std::string	text = output + "\r\n";
	send(clientSocket, text.c_str(), text.size(), 0);
}

bool	is_adminServer(Client* client)
{
	bool	admin = client->m_getAdminServer();

	if (admin == false)
		ft_send(client, "(!) You need << ADMIN SERVER >> permissions to use this command");
	return (admin);
}

bool	get_command_parameter(Server& server, Client* client)
{
	std::string str = client->m_getInput();
	int	i(0);

	while (str[i] && str[i] != ' ')
		i++;
	
	std::string	cmd = str.substr(0, i);
	if (is_upperCase(cmd) == false)
	{
		ft_send(client, "(!) Command must be capitalized");
		ft_send(client, "(i) Use /HELP for instructions");
		return (0);
	}
	client->m_setCmd(cmd);
	
	std::string parameter = "";
	if (ft_nbrSpace(str) > 0)
		parameter = str.substr(i + 1, str.size());
	client->m_setParameter(parameter);

	if (!ft_run(server, client))
		return (0);
	return (1);
}
