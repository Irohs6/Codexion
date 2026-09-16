/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:25 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/15 17:00:56 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "error.h"

int	ft_is_number_valid(char *str, int arg_position)
{
	int	i;
	int	negative;

	negative = 0;
	i = 0;

	if (!str || !str[0])
		return (print_error(1, ERR_EMPTY_MSG, arg_position));
	if (str[i] == '-')
	{
		negative = 1;
		i++;
	}
	if (!str[i])
		return (print_error(1, ERR_EMPTY_MSG, arg_position));
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (print_error(3, ERR_NUMBER_MSG, arg_position));
		i++;
	}
	if (negative == 1)
		return (print_error(2, ERR_NEGATIVE_MSG, arg_position));
	return (1);
}

int	ft_is_str_valid(char *str, int arg_position)
{
	if (!str || !str[0])
		return (print_error(1, ERR_EMPTY_MSG, arg_position));
	if (strcmp(str, "fifo") && strcmp(str, "edf"))
		return (print_error(5, ERR_SCHEDULER_MSG, arg_position));
	return (1);
}

int	ft_convert_number(char *str, int arg_position)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (result > 2147483647)
		return (print_error(4, ERR_RANGE_MSG, arg_position));
	return ((int) result);
}

