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

void	print_list_user(Client*	client, std::vector<Client *> &list_user)
{
	int	i(0);
	int	nb_clients = list_user.size();

	ft_send(client, "[   Number clients on Server: " + int_to_string(nb_clients) + "   ]");
	while (i < nb_clients)
	{
		Client* user = list_user[i];

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

void	ft_find_channel(Server& server, Client* client, std::string nameChannel)
{
	size_t	i(0);
	std::vector<Channel *> &list_channel = server.getChannels();

	while (i < list_channel.size())
	{
		if (list_channel[i]->getNameChannel() == nameChannel)
		{
			print_list_user(client, list_channel[i]->getUsers());
			return ;
		}
		i++;
	}
}

void	ft_command_who(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "")
		print_list_user(client, server.getRegisteredClients());
	else
	{
		if (ft_nbrSpace(parameter) != 0 || parameter[0] != '#')
			return (error_syntax(client));
		ft_find_channel(server, client, parameter);
	}
}
