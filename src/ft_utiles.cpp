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

bool	is_upperCase(std::string& str)
{
	int	i(0);

	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			return (0);
		i++;
	}
	return (1);
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
	ft_send(client, 4, "(!) Wrong syntax");
	ft_send(client, 3, "(i) Use /HELP for instructions");
	return ;
}

std::string	int_to_string(int nbr)
{
	std::ostringstream oss;
	oss << nbr;
	std::string str = oss.str();
	return (str);
}
