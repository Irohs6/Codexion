/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 17:24:02 by iroh              #+#    #+#             */
/*   Updated: 2026/09/30 13:49:38 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "monitoring.h"
#include "error.h"

static t_bool	finish_codexion(t_memory_manager *manager,
	const t_config *config)
{
	t_bool	status;
	int		i;

	if (pthread_join(manager->monitoring->monitor_thread, NULL) != 0)
		return (print_error(ERR_THREAD_JOIN, 0));
	if (join_threads(manager, config->number_of_coders) == FALSE)
		return (FALSE);
	status = destroy_mutexes(manager, config->number_of_coders);
	i = -1;
	while (++i < config->number_of_coders)
	{
		if (manager->array_coder[i].failed == TRUE)
			status = FALSE;
	}
	free_memory_manager(manager);
	return (status);
}

t_bool	run_codexion(const t_config *config, pthread_mutex_t *log_mutex)
{
	t_memory_manager	manager;
	t_monitoring		monitoring;

	monitoring.stop = FALSE;
	pthread_mutex_init(&monitoring.mutex, NULL);
	manager.monitoring = &monitoring;
	if (memory_manager_init(&manager, config->number_of_coders) == FALSE)
		return (FALSE);
	if (init_codexion(&manager, config, log_mutex) == FALSE)
	{
		free_memory_manager(&manager);
		return (FALSE);
	}
	if (init_monitoring(&monitoring, &manager) == FALSE)
	{
		free_memory_manager(&manager);
		return (FALSE);
	}
	return (finish_codexion(&manager, config));
}
