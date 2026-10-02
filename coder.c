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
#include "log.h"

static void	compile(t_coder *coder)
{
	if (!coder)
		return ;
	coder->last_time_compile_start = now_ms();
	display_log(coder->log_mutex, coder, MSG_DONGLE);
	display_log(coder->log_mutex, coder, MSG_DONGLE);
	display_log(coder->log_mutex, coder, MSG_COMPILE);
	usleep(coder->config->time_to_compile * 1000);
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	coder->dongle_1->last_release_time = now_ms();
	coder->dongle_2->last_release_time = coder->dongle_1->last_release_time;
	coder->dongle_1->is_available = TRUE;
	coder->dongle_2->is_available = TRUE;
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	coder->nb_compile++;
}

static void	debug(t_coder *coder)
{
	display_log(coder->log_mutex, coder, MSG_DEBUG);
	usleep(coder->config->time_to_debug * 1000);
}

static void	refactor(t_coder *coder)
{
	display_log(coder->log_mutex, coder, MSG_REFACTOR);
	usleep(coder->config->time_to_refactor * 1000);
}

void	*start_coder_thread(void *arg)
{
	t_coder	*coder;

	coder = arg;
	while (coder->nb_compile < coder->config->nb_of_cp_required)
	{
		if (coder->nb_compile > 0 && register_requests(coder) == FALSE)
		{
			coder->failed = TRUE;
			return (NULL);
		}
		while (take_dongle(coder) == FALSE)
			usleep(10);
		compile(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
