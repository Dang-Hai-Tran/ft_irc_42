/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_privmsg.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: datran <datran@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 19:02:14 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/04 18:34:16 by datran           ###   ########.fr       */
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
	message = ":" + sender + " PRIVMSG " + nameChannel + " " + message + "\r\n";
	
	size_t	i(0);
	while (i < channel->getUsers().size())
	{
		Client*	user = channel->getUsers()[i];
		std::string	recipient = user->m_getNickName();
		if (recipient != sender)
				send(user->m_getSocket(), message.c_str(), message.size(), 0);
		i++;
	}
}

int	ft_find_namechannel(Server& server, std::string nameChannel)
{
	int	i(0);

	while (i < (int)server.getChannels().size())
	{
		if (server.getChannels()[i]->getNameChannel() == nameChannel)
			return (i + 1);
		i++;
	}
	return (i);
}

void	ft_command_privmsg(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();
	// std::cout << parameter << std::endl;

	if (parameter == "" || ft_nbrSpace(parameter) < 1 || parameter[0] != '#')
		return (error_syntax(client));

	int	i(0);
	while (parameter[i] && parameter[i] != ' ')
		i++;

	std::string	nameChannel = parameter.substr(1, i);
	// std::cout << "channel name: " << nameChannel << std::endl;

	int	id = ft_find_namechannel(server, nameChannel);
	if (id == 0)
	{
		ft_send(client, "(!) This channel cannot be found");
		return ;
	}

	parameter = parameter.substr(i + 1, parameter.size());
	// std::cout << "--> message: " << parameter << std::endl;
	client->m_setParameter(parameter);

	ft_conversation(client, server.getChannels()[id - 1]);
}

void	ft_message(Client* client)
{
	std::string	cmd = client->m_getInput();
	std::string	message = ":" + cmd;
	client->m_setParameter(message);

	size_t	i(0);
	while (i < client->getChannelsUserIn().size())
	{
		Channel*	channel = client->getChannelsUserIn()[i];
		ft_conversation(client, channel);
		i++;
	}
}
