/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 14:06:31 by iroh              #+#    #+#             */
/*   Updated: 2026/10/07 11:01:26 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "log.h"
#include "utils.h"
#include "codexion.h"

t_bool	display_log(pthread_mutex_t *mutex, t_coder *coder, const char *log)
{
	pthread_mutex_lock(mutex);
	printf("%llu %d %s\n",
		(unsigned long long)(now_ms() - coder->start_time),
		coder->id, log);
	pthread_mutex_unlock(mutex);
	return (TRUE);
}

t_bool	display_compile_log(pthread_mutex_t *mutex, t_coder *coder)
{
	uint64_t	time;

	if (!mutex || !coder)
		return (FALSE);
	pthread_mutex_lock(mutex);
	time = now_ms() - coder->start_time;
	printf("%llu %d %s\n",
		(unsigned long long)time, coder->id, MSG_DONGLE);
	printf("%llu %d %s\n",
		(unsigned long long)time, coder->id, MSG_DONGLE);
	printf("%llu %d %s\n",
		(unsigned long long)time, coder->id, MSG_COMPILE);
	pthread_mutex_unlock(mutex);
	return (TRUE);
}