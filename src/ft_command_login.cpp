#include "../inc/irc.hpp"

void	ft_command_login(Server& server, Client* &client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	int	id = ft_find_username(server, parameter);
	if (id == 0)
	{
		ft_send(client, 4, "(!) This account name never existed");
		ft_send(client, 4, "(i) Use /USER to register");
		ft_send(client, 4, "Command: /USER username 0 * :realname");
		return ;
	}

	ft_send(client, 4, "<--   Welcome back (" + parameter + ")   -->");

	Client*	oldClient = server.getRegisteredClients()[id - 1];
	oldClient->m_setSocket(client->m_getSocket());
	oldClient->m_setConnected(true);
	oldClient->m_setStatusS(true);

	delete client;
	client = oldClient;
}
