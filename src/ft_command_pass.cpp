#include "../inc/irc.hpp"

void	ft_command_pass(Server& server, Client* client)
{
	std::string	parameter = client->m_getParameter();

	if (parameter == "" || ft_nbrSpace(parameter) != 0)
		return (error_syntax(client));

	if (parameter != server.getPassword())
	{
		ft_send(client, 4, "(!) Wrong password");
		return ;
	}
	// ft_send(client, 4, "|//////////////////////////////////////|\r");
	// ft_send(client, 4, "|//     Welcome to my server IRC     //|\r");
	// ft_send(client, 4, "|//////////////////////////////////////|\r");
	client->m_setConnected(true);
}
