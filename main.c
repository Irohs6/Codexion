/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 16:47:24 by iroh             ###   ########.fr       */
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
	t_config		config;

	if (argc == 9)
	{
		if (parse(argc, argv, &config) == -1)
			return (1);
		printf("%d\n", config.number_of_coders);
		printf("%d\n", config.time_to_burnout);
		printf("%d\n", config.time_to_compile);
		printf("%d\n", config.time_to_debug);
		printf("%d\n", config.time_to_refactor);
		printf("%d\n", config.number_of_compiles_required);
		printf("%d\n", config.dongle_cooldown);
		printf("%s\n", config.scheduler);
	}
	else
	{
		print_error(4, ERR_ARG_COUNT_MSG, 0);
		return (1);
	}
	return (0);
}
