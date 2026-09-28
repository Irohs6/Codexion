/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/28 14:20:22 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "error.h"
#include "codexion.h"
#include "memory_manager.h"
#include <pthread.h>

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

static t_bool	run_codexion(const t_config *config, pthread_mutex_t *log_mutex)
{
	t_memory_manager	manager;
	t_bool				status;
	int					i;

	if (memory_manager_init(&manager, config->number_of_coders) == FALSE)
		return (FALSE);
	if (init_codexion(&manager, config, log_mutex) == FALSE)
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
	t_config		config;
	pthread_mutex_t	log_mutex;
	t_bool			status;

	if (argc != 9)
	{
		print_error(ERR_ARG_COUNT, 0);
		return (EXIT_FAILURE);
	}
	if (parse(argc, argv, &config) == FALSE)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&log_mutex, NULL) != 0)
	{
		print_error(ERR_MUTEX, 0);
		return (EXIT_FAILURE);
	}
	status = run_codexion(&config, &log_mutex);
	if (pthread_mutex_destroy(&log_mutex) != 0)
		status = print_error(ERR_MUTEX_DESTROY, 0);
	if (status == FALSE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
