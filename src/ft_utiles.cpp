/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utiles.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:19:22 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/28 17:19:25 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

/* Check string empty */
bool	ft_isStringEmpty(const std::string& str)
{
	int	i;

	if (str == "")
		return (true);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (false);
		i++;
	}
	return (true);
}

/* Delete spaces */
std::string	str_without_space_start_end(const std::string& arg)
{
	int			start(0);
	int			end(0);

	while (arg[start] && (arg[start] == ' ' || arg[start] == '\t'))
		start++;
	end = arg.size() - 1;
	while (end >= 0 && (arg[end] == ' ' || arg[end] == '\t'))
		end--;
	return (arg.substr(start, end - start + 1));
}

std::string	upperCase(std::string& str)
{
	int	i(0);
	std::string	new_str = "";

	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			new_str += str[i] - 32;
		else
			new_str += str[i];
		i++;
	}
	return (new_str);
}

int	ft_nbrSpace(const std::string& str)
{
	int	i(0);
	int	nbr_space(0);

	while (str[i])
	{
		if (str[i] == ' ')
			nbr_space++;
		i++;
	}
	return (nbr_space);
}

void	error_syntax(Client* client)
{
	std::string	nickName = client->m_getNickName();
	std::string	command = client->m_getCmd();

	if (nickName == "")
		nickName = int_to_string(client->m_getSocket());
	ft_send(client, ERR_NEEDMOREPARAMS(nickName, command));
	return ;
}

std::string	int_to_string(int nbr)
{
	std::ostringstream oss;
	oss << nbr;
	std::string str = oss.str();
	return (str);
}

void    send_status(Client* client)
{
	std::string text = "";
    text += "[" + client->m_getNickName();
    for (size_t i = 0; i < client->getChannelsUserIn().size(); i++) {
        Channel *channel = client->getChannelsUserIn()[i];
        text += " " + channel->getNameChannel();
    }
    text += "] ";
    if (client->m_getNickName() == "")
        text = "[irc] ";
    send(client->m_getSocket(), text.c_str(), text.size(), 0);
}
