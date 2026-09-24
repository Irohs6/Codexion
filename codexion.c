/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:32:40 by iroh              #+#    #+#             */
/*   Updated: 2026/09/24 22:00:52 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "error.h"

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
		manager->array_coder[i].dongle_1 = &manager->array_dongle[i];
		manager->array_coder[i].dongle_2 = &manager->array_dongle[
			(i + 1) % config->number_of_coders];
		manager->array_dongle[i].id = i + 1;
		manager->array_dongle[i].config = config;
		i++;
	}
	if (init_mutexes(manager, config->number_of_coders) != 0)
		return (-1);
	if (create_threads(manager, config->number_of_coders) != 0)
		return (-1);
	printf("All threads and mutexes initialized successfully.\n");
	return (0);
}

void	*start_coder_thread(void *coder)
{
	printf("Starting coder thread for coder ID: %d\n", ((t_coder *)coder)->id);
	return (NULL);
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
