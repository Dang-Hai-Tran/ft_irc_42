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

void	ft_send(Client& client, int type, std::string output)
{
	int	clientSocket = client.m_getSocket();

	std::string text = "";
	if (type == 1)
		text = output;
	else if (type == 2)
		text = "	" + output;
	else if (type == 3)
		text = output + "\n";
	else if (type == 4)
		text = "	" + output + "\n";
	send(clientSocket, text.c_str(), text.size(), 0);
}

bool	is_adminServer(Client& client)
{
	bool	admin = client.m_getAdminServer();

	if (admin == false)
		ft_send(client, 4, "(!) You need << ADMIN SERVER >> permissions to use this command");
	return (admin);
}

void	get_command_parameter(Server& server, Client& client)
{
	std::string str = client.m_getInput();
	int	i(0);

	while (str[i] && str[i] != ' ')
		i++;
	
	std::string	cmd = str.substr(1, i - 1);
	if (is_upperCase(cmd) == false)
	{
		ft_send(client, 4, "(!) Command must be capitalized");
		return ;
	}
	client.m_setCmd(cmd);
	
	std::string parameter = "";
	if (ft_nbrSpace(str) > 0)
		parameter = str.substr(i + 1, str.size());
	client.m_setParameter(parameter);

	ft_run(server, client);
}

void	ft_listening(Server& server, Client& client)
{

	/***********************************/
	static int i(0);
	static std::string in = "";
	std::string	input = client.m_getInput();

	i++;
	if (i == 1)
		in = input;
	else if (i == 2)
	{
		in += input;
		client.m_setInput(in);
		get_input(server, client);
		i = 0;
		in = "";
	}
	/***********************************/
}
