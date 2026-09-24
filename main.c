/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/23 16:10:00 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include "codexion.h"
#include "memory_manager.h"

static int	join_and_destroy(t_memory_manager *manager, int count)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < count)
	{
		if (pthread_join(manager->array_coder[i].thread, NULL) != 0)
			status = -1;
		i++;
	}
	if (status == -1)
		return (print_error(10, "thread join failed", 0));
	i = 0;
	while (i < count)
	{
		if (pthread_mutex_destroy(&manager->array_dongle[i].mutex) != 0)
			status = -1;
		i++;
	}
	if (status == -1)
		return (print_error(10, "mutex destruction failed", 0));
	return (0);
}

static int	run_codexion(const t_config *config)
{
	t_memory_manager	manager;

	if (memory_manager_init(&manager, config->number_of_coders) == -1)
		return (1);
	if (init_codexion(&manager, config) == -1)
	{
		free_memory_manager(&manager);
		return (1);
	}
	if (join_and_destroy(&manager, config->number_of_coders) == -1)
		return (1);
	free_memory_manager(&manager);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	config;

	if (argc != 9)
	{
		print_error(4, ERR_ARG_COUNT_MSG, 0);
		return (1);
	}
	if (parse(argc, argv, &config) == -1)
		return (1);
	if (config.number_of_coders == 0)
	{
		print_error(6, ERR_ZERO_MSG, 1);
		return (1);
	}
	return (run_codexion(&config));
}
