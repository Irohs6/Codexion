/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:58:00 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:58:04 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "error.h"

static void	clean_thread_failure(t_memory_manager *manager,
	int nb_created, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_created)
	{
		pthread_join(manager->array_coder[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < nb_coder)
	{
		pthread_mutex_destroy(&manager->array_dongle[i].mutex);
		i++;
	}
}

t_bool	create_threads(struct s_memory_manager *manager,
	int nb_coder, pthread_mutex_t *log_mutex)
{
	int	i;

	i = -1;
	if (!manager || !manager->array_coder || !log_mutex)
		return (print_error(ERR_NULL, 0));
	if (register_initial_requests(manager, nb_coder) == FALSE)
	{
		clean_thread_failure(manager, 0, nb_coder);
		return (FALSE);
	}
	while (++i < nb_coder)
	{
		manager->array_coder[i].log_mutex = log_mutex;
		if (pthread_create(&manager->array_coder[i].thread, NULL,
				start_coder_thread, &manager->array_coder[i]) != 0)
		{
			print_error(ERR_THREAD, 0);
			clean_thread_failure(manager, i, nb_coder);
			return (FALSE);
		}
	}
	return (TRUE);
}

t_bool	join_threads(t_memory_manager *manager, int count)
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

t_bool	destroy_mutexes(t_memory_manager *manager, int count)
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
