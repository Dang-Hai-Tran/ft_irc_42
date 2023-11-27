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

void	ft_connected_successfully(Client* client)
{
	std::string	userName = client->m_getUserName();
	std::string	nickName = client->m_getNickName();
	std::string	host = client->m_getRealName();

	std::string	text = ":localhost 001 " + nickName + " :Welcome to the IRC Network " + nickName + "!" + userName + "@" + host + "\r\n";
	send(client->m_getSocket(), text.c_str(), text.size(), 0);
}

void	ft_irssi_get_username(Server& server, Client* client)
{
	(void)server;
	std::string	parameter = client->m_getParameter();
	int	nbr_space = ft_nbrSpace(parameter);
	int	compter(0);
	int	i(0);

	while (parameter[i] && compter < nbr_space)
	{
		if (parameter[i] == ' ')
			compter++;
		i++;
	}
	std::string	userName = parameter.substr(0, i - 1);
	client->m_setUserName(userName);

	// created_successfully(server, client);
	ft_connected_successfully(client);
}

void	ft_irssi_get_realname(Server& server, Client* client)
{
	(void)server;
	std::string	parameter = client->m_getParameter();

	size_t	i = parameter.size() - 1;

	while (i >= 0 && parameter[i] != ':')
		i--;
	std::string	realName = parameter.substr(i + 1, parameter.size());
	client->m_setRealName(realName);
	
	parameter = parameter.substr(0, i - 1);
	client->m_setParameter(parameter);
	ft_irssi_get_username(server, client);
}

void	ft_irssi_get_input(Server& server, Client* client)
{
	(void)server;
	std::string	str = client->m_getInput();

	int	i(0);

	while (str[i] && str[i] != ' ')
		i++;
	std::string	cmd = str.substr(0, i);
	client->m_setCmd(cmd);
	std::string	parameter = str.substr(i + 1, str.size());
	client->m_setParameter(parameter);

	if (cmd == "PASS")
		ft_command_pass(server, client);
	else if (cmd == "NICK")
		ft_command_nick(server, client);
	else if (cmd == "USER")
		ft_irssi_get_realname(server, client);
	else
	{
		std::cout << str << std::endl;
		// std::cout << "--> cmd: " << cmd << std::endl;
		// std::cout << "--> parameter: " << parameter << std::endl;
	}
}
