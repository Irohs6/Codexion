/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:32:40 by iroh              #+#    #+#             */
/*   Updated: 2026/09/28 14:21:25 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "error.h"

static void	init_dongle_id(t_memory_manager *manager, int nb_coders, int i)
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
		init_dongle_id(manager, config->number_of_coders, i);
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

static void	clean_thread_failure(t_memory_manager *manager,
	int created, int count)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(manager->array_coder[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&manager->array_dongle[i].mutex);
		i++;
	}
}

t_bool	create_threads(struct s_memory_manager *manager,
	int nb_coder, pthread_mutex_t *log_mutex)
{
	int	i;

	i = 0;
	if (!manager || !manager->array_coder || !log_mutex)
		return (print_error(ERR_NULL, 0));
	while (i < nb_coder)
	{
		manager->array_coder[i].log_mutex = log_mutex;
		if (pthread_create(&manager->array_coder[i].thread, NULL,
				start_coder_thread, &manager->array_coder[i]) != 0)
		{
			print_error(ERR_THREAD, 0);
			clean_thread_failure(manager, i, nb_coder);
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

t_bool	init_mutexes(struct s_memory_manager *manager, int nb_dongle)
{
	int	i;

	i = 0;
	if (!manager || !manager->array_dongle)
		return (print_error(ERR_NULL, 0));
	while (i < nb_dongle)
	{
		if (pthread_mutex_init(&manager->array_dongle[i].mutex, NULL) != 0)
		{
			print_error(ERR_MUTEX, 0);
			while (--i >= 0)
			{
				pthread_mutex_destroy(&manager->array_dongle[i].mutex);
			}
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}
