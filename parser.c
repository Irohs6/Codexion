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

int	ft_is_number_valid(char *str, int arg_position)
{
	int	i;
	int	negative;

	negative = 0;
	i = 0;

	if (!str)
		print_error(1, "NULL string provided", arg_position);
	if (str[i] == '-')
	{
		negative = 1;
		i++;
	}
	if (!str[i])
		print_error(1, "Empty string after minus sign", arg_position);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			print_error(3, "Invalid number", arg_position);
		i++;
	}
	if (negative == 1)
		print_error(2, "negative numbers are not allowed", arg_position);
	return (1);
}

void	ft_is_str_valid(char *str, int arg_position)
{
	if (!str || !str[0])
		print_error(1, "NULL string provided", arg_position);
	if (strcmp(str, "fifo") && strcmp(str, "edf"))
		print_error(5, "Invalid string must be fifo or edf is valid", arg_position);
}

int	ft_convert_number(char *str, int arg_position)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	if (ft_strlen(str) >= 11)
		print_error(4, "Number out of range", arg_position);
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (result > 2147483647)
		print_error(4, "Number out of range", arg_position);
	return ((int) result);
}