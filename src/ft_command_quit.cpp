/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_quit.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:40:35 by xuluu             #+#    #+#             */
/*   Updated: 2023/12/12 12:40:53 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

bool	ft_command_quit(Client* client)
{
	std::string	parameter = client->m_getParameter();
	std::string	nickName = client->m_getNickName();
	std::string	userName = client->m_getUserName();

	if (parameter.size() > 0 && parameter[0] != ':')
		return (0);
	ft_send(client, RPL_QUIT(user_id(nickName, userName), parameter));
	return (1);
}
