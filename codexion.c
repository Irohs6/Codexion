/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:32:40 by iroh              #+#    #+#             */
/*   Updated: 2026/09/25 21:58:05 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "error.h"

static void	init_dongle_id(t_memory_manager *manager, int nb_coders, int i)
{
	t_dongle	*tmp;

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

int	init_codexion(t_memory_manager *manager, const t_config *config)
{
	int	i;

	if (!manager || !config)
		return (-1);
	if (!manager->array_coder || !manager->array_dongle)
		return (-1);
	i = 0;
	while (i < config->number_of_coders)
	{
		manager->array_coder[i].id = i + 1;
		manager->array_coder[i].config = config;
		init_dongle_id(manager, config->number_of_coders, i);
		manager->array_dongle[i].id = i + 1;
		manager->array_dongle[i].config = config;
		manager->array_dongle[i].is_available = 1;
		i++;
	}
	if (init_mutexes(manager, config->number_of_coders) != 0)
		return (-1);
	init_start_time(manager, config->number_of_coders);
	if (create_threads(manager, config->number_of_coders) != 0)
		return (-1);
	return (0);
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

int	create_threads(struct s_memory_manager *manager, int nb_coder)
{
	int	i;

	i = 0;
	if (!manager || !manager->array_coder)
		return (-1);
	while (i < nb_coder)
	{
		if (pthread_create(&manager->array_coder[i].thread, NULL,
				start_coder_thread, &manager->array_coder[i]) != 0)
		{
			print_error(10, ERR_THREAD_MSG, 0);
			clean_thread_failure(manager, i, nb_coder);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_mutexes(struct s_memory_manager *manager, int nb_dongle)
{
	int	i;

	i = 0;
	if (!manager || !manager->array_dongle)
		return (-1);
	while (i < nb_dongle)
	{
		if (pthread_mutex_init(&manager->array_dongle[i].mutex, NULL) != 0)
		{
			print_error(10, ERR_MUTEX_MSG, 0);
			while (--i >= 0)
			{
				pthread_mutex_destroy(&manager->array_dongle[i].mutex);
			}
			return (-1);
		}
		i++;
	}
	return (0);
}
