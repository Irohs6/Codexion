/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 13:48:58 by iroh              #+#    #+#             */
/*   Updated: 2026/09/30 13:50:07 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "monitoring.h"
#include "log.h"


static void	compile(t_coder *coder)
{
	t_bool	completed;

	if (!coder)
		return ;
	pthread_mutex_lock(&coder->monitoring->mutex);
	if (coder->monitoring->stop == TRUE)
	{
		pthread_mutex_unlock(&coder->monitoring->mutex);
		return ;
	}
	coder->last_time_compile_start = now_ms();
	pthread_mutex_unlock(&coder->monitoring->mutex);
	display_log(coder->log_mutex, coder, MSG_DONGLE);
	display_log(coder->log_mutex, coder, MSG_DONGLE);
	display_log(coder->log_mutex, coder, MSG_COMPILE);
	completed = wait_phase(coder, coder->config->time_to_compile);
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	coder->dongle_1->last_release_time = now_ms();
	coder->dongle_2->last_release_time = coder->dongle_1->last_release_time;
	coder->dongle_1->is_available = TRUE;
	coder->dongle_2->is_available = TRUE;
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	pthread_mutex_lock(&coder->monitoring->resource_mutex);
	pthread_cond_broadcast(&coder->monitoring->resource_cond);
	pthread_mutex_unlock(&coder->monitoring->resource_mutex);
	if (completed == FALSE)
		return ;
	pthread_mutex_lock(&coder->monitoring->mutex);
	coder->nb_compile++;
	pthread_mutex_unlock(&coder->monitoring->mutex);
}

static t_bool	debug(t_coder *coder)
{
	if (monitoring_stoped(coder->monitoring) == TRUE)
		return (FALSE);
	display_log(coder->log_mutex, coder, MSG_DEBUG);
	return (wait_phase(coder, coder->config->time_to_debug));
}

static t_bool	refactor(t_coder *coder)
{
	if (monitoring_stoped(coder->monitoring) == TRUE)
		return (FALSE);
	display_log(coder->log_mutex, coder, MSG_REFACTOR);
	return (wait_phase(coder, coder->config->time_to_refactor));
}

void	*start_coder_thread(void *arg)
{
	t_coder	*coder;

	coder = arg;
	while (monitoring_stoped(coder->monitoring) == FALSE
		&& coder->nb_compile < coder->config->nb_of_cp_required)
	{
		if (coder->nb_compile > 0 && register_requests(coder) == FALSE)
		{
			coder->failed = TRUE;
			return (NULL);
		}
		while (monitoring_stoped(coder->monitoring) == FALSE
			&& take_dongle(coder) == FALSE)
		{
			pthread_mutex_lock(&coder->monitoring->resource_mutex);
			pthread_cond_wait(&coder->monitoring->resource_cond,
				&coder->monitoring->resource_mutex);
			usleep(coder->config->dongle_cooldown * 1000);
			pthread_mutex_unlock(&coder->monitoring->resource_mutex);
		}
		if (monitoring_stoped(coder->monitoring) == TRUE)
			return (NULL);
		compile(coder);
		if (coder->nb_compile >= coder->config->nb_of_cp_required)
			return (NULL);
		if (debug(coder) == FALSE)
			return (NULL);
		if (refactor(coder) == FALSE)
			return (NULL);
	}
	return (NULL);
}
