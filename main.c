/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/21 13:09:06 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "parser.h"
#include "config.h"
#include "error.h"
#include "codexion.h"
#include "memory_manager.h"


int	main(int argc, char **argv)
{
	t_config			config;
	t_memory_manager	memory_manager;
	int	i;

	i = 0;

	if (argc == 9)
	{
		if (parse(argc, argv, &config) == -1)
			return (1);
		if (memory_manager_init(&memory_manager, config.number_of_coders) == -1)
			return (1);
		if (init_codexion(&memory_manager, &config) == -1)
			return (1);
		while (i <config.number_of_coders)
		{
			printf("Coder %d:\n", memory_manager.array_coder[i].id);
			i++;
		}
		free_memory_manager(&memory_manager);
	}
	else
	{
		print_error(4, ERR_ARG_COUNT_MSG, 0);
		return (1);
	}
	return (0);
}
