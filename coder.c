/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:54:44 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 16:32:04 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "monitoring.h"
#include "log.h"

static t_bool	start_compile(t_coder *coder)
{
	uint64_t	now;

	pthread_mutex_lock(&coder->monitoring->mutex);
	now = now_ms();
	if (coder->monitoring->stop == TRUE
		|| now >= coder->last_time_compile_start
		+ coder->config->time_to_burnout)
	{
		pthread_mutex_unlock(&coder->monitoring->mutex);
		return (FALSE);
	}
	coder->last_time_compile_start = now;
	pthread_mutex_unlock(&coder->monitoring->mutex);
	return (TRUE);
}

static t_bool	compile(t_coder *coder)
{
	t_bool	completed;

	if (start_compile(coder) == FALSE)
		return (FALSE);
	display_compile_log(coder->log_mutex, coder);
	completed = wait_phase(coder, coder->config->time_to_compile);
	release_dongles(coder);
	if (completed == FALSE)
		return (FALSE);
	pthread_mutex_lock(&coder->monitoring->mutex);
	coder->nb_compile++;
	pthread_mutex_unlock(&coder->monitoring->mutex);
	return (TRUE);
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
	t_coder		*coder;

	coder = arg;
	while (monitoring_stoped(coder->monitoring) == FALSE
		&& coder->nb_compile < coder->config->nb_of_cp_required)
	{
		if (coder->nb_compile > 0 && register_requests(coder) == FALSE)
		{
			coder->failed = TRUE;
			return (NULL);
		}
		if (wait_for_dongles(coder) == FALSE)
			return (NULL);
		if (monitoring_stoped(coder->monitoring) == TRUE)
			return (NULL);
		if (compile(coder) == FALSE)
			return (NULL);
		if (coder->nb_compile >= coder->config->nb_of_cp_required)
			return (NULL);
		if (debug(coder) == FALSE)
			return (NULL);
		if (refactor(coder) == FALSE)
			return (NULL);
	}
	return (NULL);
}
