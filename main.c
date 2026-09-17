/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 15:17:50 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "config.h"
#include "utils.h"
#include "error.h"

int	main(int argc, char **argv)
{
	t_config config;

	if (argc == 9)
	{
		if (parse(argc, argv, &config) == -1)
			return (1);
	}
	else
	{
		print_error(4, ERR_ARG_COUNT_MSG, 0);
		return (1);
	}
	return (0);
}
