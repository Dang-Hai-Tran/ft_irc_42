/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delete_space.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuluu <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:18:07 by xuluu             #+#    #+#             */
/*   Updated: 2023/11/28 17:18:09 by xuluu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/irc.hpp"

void	ft_delete_space2(std::string& result, std::string& str)
{
	int		i(0);
	bool	start_message(0);
	bool	is_space(0);

	i = 0;
	while (str[i])
	{
		if (str[i] == ':')
			start_message = 1;
		if (start_message == 1)
			result += str[i];
		else if (str[i] != ' ')
		{
			is_space = 0;
			result += str[i];
		}
		else if (is_space == 0)
		{
			result += ' ';
			is_space = 1;
		}
		i++;
	}
}

std::string	ft_delete_space(const std::string& input)
{
	std::string str = input;
	std::string result = "";
	
	int	i(0);

	// remplace '\t' --> ' '
	while (str[i] && str[i] != ':')
	{
		if (str[i] == '\t')
			str[i] = ' ';
		i++;
	}

	// put 1 space between character
	ft_delete_space2(result, str);

	return (result);
}
