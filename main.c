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

static t_bool	join_threads(t_memory_manager *manager, int count)
{
	int		i;
	t_bool	status;

	i = 0;
	status = TRUE;
	while (i < count)
	{
		if (pthread_join(manager->array_coder[i].thread, NULL) != 0)
			status = FALSE;
		i++;
	}
	if (status == FALSE)
		return (print_error(ERR_THREAD_JOIN, 0));
	return (TRUE);
}

static t_bool	destroy_mutexes(t_memory_manager *manager, int count)
{
	int		i;
	t_bool	status;

	i = 0;
	status = TRUE;
	while (i < count)
	{
		if (pthread_mutex_destroy(&manager->array_dongle[i].mutex) != 0)
			status = FALSE;
		i++;
	}
	if (status == FALSE)
		return (print_error(ERR_MUTEX_DESTROY, 0));
	return (TRUE);
}

static t_bool	run_codexion(const t_config *config)
{
	t_memory_manager	manager;
	t_bool				status;
	int					i;

	if (memory_manager_init(&manager, config->number_of_coders) == FALSE)
		return (FALSE);
	if (init_codexion(&manager, config) == FALSE)
	{
		free_memory_manager(&manager);
		return (FALSE);
	}
	if (join_threads(&manager, config->number_of_coders) == FALSE)
		return (FALSE);
	status = destroy_mutexes(&manager, config->number_of_coders);
	i = -1;
	while (++i < config->number_of_coders)
	{
		if (manager.array_coder[i].failed == TRUE)
			status = FALSE;
	}
	free_memory_manager(&manager);
	return (status);
}

int	main(int argc, char **argv)
{
	t_config	config;

	if (argc != 9)
	{
		print_error(ERR_ARG_COUNT, 0);
		return (EXIT_FAILURE);
	}
	if (parse(argc, argv, &config) == FALSE)
		return (EXIT_FAILURE);
	if (run_codexion(&config) == FALSE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
