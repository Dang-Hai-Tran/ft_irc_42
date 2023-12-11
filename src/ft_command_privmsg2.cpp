/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_privmsg2.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:08:00 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/11 16:08:30 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_send_a_person2(Server& server, Client* client, int id)
{
	Client*	target = server.getRegisteredClients()[id];
	std::string	sender = client->m_getNickName();
	std::string	message = client->m_getParameter();
	std::string annonce = "(!) You have a private message from: " + sender;

	size_t	nbr_channel = target->getChannelsUserIn().size();
	if (nbr_channel == 0)
	{
		ft_send(target, annonce + "\r\n");
		ft_send(target, sender + ": " + message + "\r\n");
		return ;
	}

	size_t	i(0);
	while (i < nbr_channel)
	{
		std::string	nameChannel = target->getChannelsUserIn()[i]->getNameChannel();
		ft_send(target, RPL_PRIVMSG2(sender, nameChannel, annonce));
		ft_send(target, RPL_PRIVMSG2(sender, nameChannel, message));
		i++;
	}
}

void	ft_send_a_person(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	size_t	i(0);
	while (parameter[i] && parameter[i] != ' ')
		i++;
	std::string	nickName = parameter.substr(0, i);

	std::string	message = parameter.substr(i + 1, parameter.size());
	if (message.size() < 2 || message[0] != ':')
		return (error_syntax(client));
	message = parameter.substr(i + 2, parameter.size());
	client->m_setParameter(message);

	int	id = ft_find_nickname(server, nickName);
	if (id == 0)
	{
		ft_send(client, ERR_NOSUCHNICK(client->m_getNickName(), parameter));
		return ;
	}
	if (id != client->m_getID())
		ft_send_a_person2(server, client, id - 1);
}
