/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:57:19 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 16:00:21 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring.h"
#include "codexion.h"
#include "memory_manager.h"
#include "error.h"
#include "log.h"

static t_bool	handle_burnout(t_coder *array_coder,
	t_monitoring *monitoring)
{
	int	burnout_id;

	burnout_id = check_all_deadline(array_coder,
			array_coder[0].config->number_of_coders);
	if (burnout_id == FALSE)
		return (FALSE);
	stop_monitoring(monitoring);
	display_log(array_coder[burnout_id - 1].log_mutex,
		&array_coder[burnout_id - 1], MSG_BURNOUT);
	return (TRUE);
}

t_bool	monitoring_stoped(t_monitoring *monitoring)
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
	pthread_mutex_lock(&monitoring->resource_mutex);
	pthread_cond_broadcast(&monitoring->resource_cond);
	pthread_mutex_unlock(&monitoring->resource_mutex);
}

void	*monitoring_function(void *arg)
{
	int					burnout_id;
	t_memory_manager	*manager;
	t_monitoring		*monitoring;
	t_coder				*array_coder;

	manager = (t_memory_manager *)arg;
	monitoring = manager->monitoring;
	array_coder = manager->array_coder;
	while (monitoring_stoped(manager->monitoring) == FALSE)
	{
		if (all_compiles_done(array_coder,
				array_coder[0].config->number_of_coders) == TRUE)
		{
			stop_monitoring(monitoring);
			break ;
		}
		burnout_id = check_all_deadline(array_coder,
				array_coder[0].config->number_of_coders);
		if (handle_burnout(array_coder, monitoring) == TRUE)
			break ;
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
