/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_privmsg.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:14 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/12 18:39:34 by datran           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_conversation(Client* client, Channel* channel)
{
	std::string	message = client->m_getParameter();

	if (message.size() < 2 || message[0] != ':')
		return (error_syntax(client));

	std::string	sender = client->m_getNickName();
	std::string	nameChannel = channel->getNameChannel();

	if (!client->m_isMessage())
	{
		ft_send(client, RPL_PRIVMSG2(sender, nameChannel, message));
		return ;
	}
	
	size_t	i(0);
	while (i < channel->getUsers().size())
	{
		Client*	target = channel->getUsers()[i];
		std::string	recipient = target->m_getNickName();
		if (recipient != sender)
			ft_send(target, RPL_PRIVMSG2(sender, nameChannel, message));
		i++;
	}
	if (!client->m_usingIrssi())
		send_status(client);
}

int	ft_find_namechannel(Server& server, std::string nameChannel)
{
	size_t	i(0);

	while (i < server.getChannels().size())
	{
		if (server.getChannels()[i]->getNameChannel() == nameChannel)
			return (i + 1);
		i++;
	}
	return (0);
}

void	ft_command_privmsg(Server& server, Client* client)
{
	client->m_defineMessage(true);
	std::string	parameter = client->m_getParameter();
	if (parameter == "" || ft_nbrSpace(parameter) < 1)
		return (error_syntax(client));

	if (parameter[0] != '#')
	{
		ft_send_a_person(server, client);
		return ;
	}

	if (!client->m_usingIrssi())
	{
		ft_send(client, ERR_UNKNOWNCOMMAND(client->m_getNickName(), client->m_getCmd()));
		return ;
	}

	int	i(0);
	while (parameter[i] && parameter[i] != ' ')
		i++;

	std::string	nameChannel = parameter.substr(0, i);

	int	id = ft_find_namechannel(server, nameChannel);
	if (id == 0)
	{
		ft_send(client, ERR_NOSUCHCHANNEL(client->m_getNickName(), nameChannel));
		return ;
	}

	parameter = parameter.substr(i + 1, parameter.size());
	client->m_setParameter(parameter);

	ft_conversation(client, server.getChannels()[id - 1]);
}

void	ft_message(Client* client, std::string cmd)
{
	std::string	message = ":" + cmd;
	client->m_setParameter(message);

	size_t	nbr_channel = client->getChannelsUserIn().size();
	if (nbr_channel == 0)
	{
		ft_send(client, cmd + "\r\n");
		return ;
	}

	size_t	i(0);
	while (i < nbr_channel)
	{
		Channel*	channel = client->getChannelsUserIn()[i];
		ft_conversation(client, channel);
		break ;
		i++;
	}
}
