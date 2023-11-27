#include "../inc/irc.hpp"

void	ft_requestPassword(Server& server, Client* client)
{
	std::string cmd = client->m_getCmd();

	if (cmd != "PASS")
	{
		ft_send(client, 4, "(i) You need to login with /PASS to continue the command");
		ft_send(client, 4, "Command: /PASS server_password");
		return ;
	}
	ft_command_password(server, client);
}

void	ft_requestUserName(Server& server, Client* &client)
{
	std::string cmd = client->m_getCmd();

	if (cmd == "LOGIN")
		ft_command_login(server, client);
	else if (cmd == "USER")
		ft_command_username(server, client);
	else
	{
		ft_send(client, 4, "(i) You need a USERNAME to continue the command !");
		ft_send(client, 4, "Command: /USER username 0 * :realname");
		return ;
	}
}

void	ft_requestNickName(Server& server, Client* client)
{
	std::string cmd = client->m_getCmd();

	if (cmd == "NICK")
		ft_command_nickname(server, client);
	else if (cmd == "USER")
		ft_command_username(server, client);
	else
	{
		ft_send(client, 4, "(i) You need a NICKNAME to continue the command !");
		ft_send(client, 4, "Command: /NICK your_nickname");
		return ;
	}
}
