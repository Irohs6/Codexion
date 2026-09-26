/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 14:14:31 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/26 18:09:15 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"

static t_bool	ft_is_number_valid(char *str, int arg_position)
{
	int		i;
	t_bool	negative;

	negative = FALSE;
	i = 0;
	if (!str || !str[0])
		return (print_error(ERR_EMPTY, arg_position));
	if (str[i] == '-')
	{
		negative = TRUE;
		i++;
	}
	if (!str[i])
		return (print_error(ERR_EMPTY, arg_position));
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (print_error(ERR_NUMBER, arg_position));
		i++;
	}
	if (negative == TRUE)
		return (print_error(ERR_NEGATIVE, arg_position));
	return (TRUE);
}

static t_bool	ft_is_str_valid(char *str, int arg_position)
{
	if (!str)
		return (print_error(ERR_NULL, arg_position));
	if (!str[0])
		return (print_error(ERR_EMPTY, arg_position));
	if (strcmp(str, "fifo") && strcmp(str, "edf"))
		return (print_error(ERR_SCHEDULER, arg_position));
	return (TRUE);
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
			return (print_error(ERR_RANGE, arg_position));
	}
	return ((int)result);
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
		config->nb_of_cp_required = value;
	else if (arg_position == 7)
		config->dongle_cooldown = value;
}

t_bool	parse(int argc, char **argv, t_config *config)
{
	int	index;
	int	number;

	if (!argv || !config)
		return (print_error(ERR_NULL, 0));
	index = 1;
	while (index != argc - 1)
	{
		if (ft_is_number_valid(argv[index], index) == FALSE)
			return (FALSE);
		number = ft_convert_number(argv[index], index);
		if (number == FALSE)
			return (FALSE);
		save_structure(config, number, index);
		index++;
	}
	if (ft_is_str_valid(argv[index], index) == FALSE)
		return (FALSE);
	config->scheduler = argv[index];
	if (config->number_of_coders == 0)
		return (print_error(ERR_ZERO, 1));
	return (TRUE);
}
