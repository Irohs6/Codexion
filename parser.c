/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:25 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 15:46:10 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "error.h"
#include "config.h"

static int	ft_is_number_valid(char *str, int arg_position)
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

static int	ft_is_str_valid(char *str, int arg_position)
{
	if (!str || !str[0])
		return (print_error(1, ERR_EMPTY_MSG, arg_position));
	if (strcmp(str, "fifo") && strcmp(str, "edf"))
		return (print_error(5, ERR_SCHEDULER_MSG, arg_position));
	return (1);
}

static int	ft_convert_number(char *str, int arg_position)
{ 
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (result > 2147483647)
			return (print_error(4, ERR_RANGE_MSG, arg_position));
	}
	return ((int) result);
}

static void	save_structure(t_config *config, int value, int arg_position)
{
	if (arg_position == 1)
		config->number_of_coders = value;
	else if (arg_position == 2)
		config->time_to_burnout = value;
	else if (arg_position == 3)
		config->time_to_compile = value;
	else if (arg_position == 4)
		config->time_to_debug = value;
	else if (arg_position == 5)
		config->time_to_refactor = value;
	else if (arg_position == 6)
		config->number_of_compiles_required = value;
	else if (arg_position == 7)
		config->dongle_cooldown = value;
}

int	parse(int argc, char **argv, t_config *config)
{	
	int	index;
	int	number;

	index = 1;
	number = 0;
	while (index != argc - 1)
	{
		if (ft_is_number_valid(argv[index], index) == -1)
			return (-1);
		number = ft_convert_number(argv[index], index);
		if (number == -1)
			return (-1);
		save_structure(config, number, index);
		index++;
	}

	if (ft_is_str_valid(argv[index], index) == -1)
		return (-1);
	config->scheduler = argv[index];
	return (1);
}
