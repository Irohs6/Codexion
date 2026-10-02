/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 14:36:46 by iroh              #+#    #+#             */
/*   Updated: 2026/10/02 16:15:51 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring.h"
#include "codexion.h"
#include "memory_manager.h"
#include "error.h"
#include "log.h"

static t_bool	monitoring_stoped(t_monitoring *monitoring)
{
	t_bool	stop;

	pthread_mutex_lock(&monitoring->mutex);
	stop = monitoring->stop;
	pthread_mutex_unlock(&monitoring->mutex);
	return (stop);
}

void	stop_monitoring(t_monitoring *monitoring)
{
	pthread_mutex_lock(&monitoring->mutex);
	monitoring->stop = TRUE;
	pthread_mutex_unlock(&monitoring->mutex);
}

static int	check_all_deadline(t_coder *array_coder, int nb_coder)
{
	int	i;

	i = 0;
	while (i < nb_coder)
	{
		if (now_ms() >= array_coder[i].last_time_compile_start
			+ array_coder[i].config->time_to_burnout)
			return (array_coder[i].id);
		i++;
	}
	return (FALSE);
}

void	*monitoring_function(void *arg)
{
	int					burnout_id;
	t_memory_manager	*manager;
	t_monitoring		*monitoring;
	t_coder				*array_coder;

	burnout_id = FALSE;
	manager = (t_memory_manager *)arg;
	monitoring = manager->monitoring;
	array_coder = manager->array_coder;


	while (monitoring_stoped(manager->monitoring) == FALSE)
	{
		burnout_id = check_all_deadline(array_coder,
				array_coder[0].config->number_of_coders);
		if (burnout_id != FALSE)
		{
			stop_monitoring(monitoring);
			display_log(array_coder[burnout_id - 1].log_mutex,
				&array_coder[burnout_id - 1], MSG_BURNOUT);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

t_bool	init_monitoring(t_monitoring *monitoring,
	t_memory_manager *manager)
{
	if (pthread_create(&monitoring->monitor_thread, NULL,
			monitoring_function, manager) != 0)
		return (print_error(ERR_THREAD, 0));
	return (TRUE);
}

