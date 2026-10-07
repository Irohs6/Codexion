/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:55:55 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:56:00 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "error.h"

static void	init_dongle_id(t_memory_manager *manager, int nb_coders,
	int i, t_monitoring *monitoring)
{
	t_dongle	*tmp;

	manager->array_coder[i].failed = FALSE;
	manager->array_coder[i].dongle_1 = &manager->array_dongle[i];
	manager->array_coder[i].dongle_2 = &manager->array_dongle[
		(i + 1) % nb_coders];
	if (manager->array_coder[i].dongle_1 > manager->array_coder[i].dongle_2)
	{
		tmp = manager->array_coder[i].dongle_1;
		manager->array_coder[i].dongle_1 = manager->array_coder[i].dongle_2;
		manager->array_coder[i].dongle_2 = tmp;
	}
	manager->array_coder[i].monitoring = monitoring;
}

static void	init_start_time(t_memory_manager *manager, int count)
{
	uint64_t	start_time;
	int			i;

	start_time = now_ms();
	i = 0;
	while (i < count)
	{
		manager->array_coder[i].start_time = start_time;
		manager->array_coder[i].last_time_compile_start = start_time;
		i++;
	}
}

t_bool	init_codexion(t_memory_manager *manager, const t_config *config,
	pthread_mutex_t *log_mutex)
{
	int	i;

	if (!manager || !config || !log_mutex)
		return (print_error(ERR_NULL, 0));
	if (!manager->array_coder || !manager->array_dongle)
		return (print_error(ERR_NULL, 0));
	i = 0;
	while (i < config->number_of_coders)
	{
		manager->array_coder[i].id = i + 1;
		manager->array_coder[i].config = config;
		init_dongle_id(manager, config->number_of_coders, i,
			manager->monitoring);
		manager->array_dongle[i].id = i + 1;
		manager->array_dongle[i].config = config;
		manager->array_dongle[i].is_available = TRUE;
		i++;
	}
	if (init_mutexes(manager, config->number_of_coders) == FALSE)
		return (FALSE);
	init_start_time(manager, config->number_of_coders);
	if (create_threads(manager, config->number_of_coders, log_mutex) == FALSE)
		return (FALSE);
	return (TRUE);
}
