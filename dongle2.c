/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:46:25 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:48:27 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	wait_for_dongles(t_coder *coder)
{
	uint64_t	wait_time;

	pthread_mutex_lock(&coder->monitoring->resource_mutex);
	while (monitoring_stoped(coder->monitoring) == FALSE
		&& take_dongle(coder) == FALSE)
	{
		wait_time = get_cooldown_wait(coder);
		if (wait_time > 0)
		{
			pthread_mutex_unlock(&coder->monitoring->resource_mutex);
			usleep(wait_time * 1000);
			pthread_mutex_lock(&coder->monitoring->resource_mutex);
		}
		else
			pthread_cond_wait(&coder->monitoring->resource_cond,
				&coder->monitoring->resource_mutex);
	}
	pthread_mutex_unlock(&coder->monitoring->resource_mutex);
	if (monitoring_stoped(coder->monitoring) == TRUE)
		return (FALSE);
	return (TRUE);
}
